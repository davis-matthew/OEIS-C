/*
 * A108769  Numbers m such that m^2 + (m+1)^2 is a semiprime.
 * https://oeis.org/A108769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer m such that m^2 + (m+1)^2 is a semiprime.
 *            Computed by brute‑force search with memoisation
 * Range:     u8 n <= 112, u16 n <= 9064, u32 n <= 9064, u64 n <= 9064,
 *            u128 n <= 9064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Prime sieve (up to 2,000,000) ---------- */
static const uint64_t PRIME_LIMIT_CONST = 2000000;

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes) return; /* already initialised */

    /* allocate marker array for sieve */
    char *is_prime = (char*)malloc(PRIME_LIMIT_CONST + 1);
    if (!is_prime) return;           /* allocation failure → abort */
    memset(is_prime, 1, PRIME_LIMIT_CONST + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= PRIME_LIMIT_CONST; ++p)
        if (is_prime[p])
            for (uint64_t m = p * p; m <= PRIME_LIMIT_CONST; m += p)
                is_prime[m] = 0;

    /* collect primes */
    primes = (uint32_t*)malloc(PRIME_LIMIT_CONST * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }

    for (uint64_t i = 2; i <= PRIME_LIMIT_CONST; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;

    free(is_prime);
}

/* ---------- Integer array helper ---------- */
typedef struct {
    uint64_t *data;
    size_t size;
    size_t cap;
} Uint64Vec;

static void vec_init(Uint64Vec *v)
{
    v->data = NULL;
    v->size = 0;
    v->cap  = 0;
}

static void vec_push(Uint64Vec *v, uint64_t val)
{
    if (v->size == v->cap) {
        size_t new_cap = v->cap ? v->cap * 2 : 16;
        uint64_t *tmp = (uint64_t*)realloc(v->data, new_cap * sizeof(uint64_t));
        if (!tmp) return; /* allocation failure → abort */
        v->data = tmp;
        v->cap  = new_cap;
    }
    v->data[v->size++] = val;
}

/* ---------- Semiprime test using the global prime list ---------- */
static int is_semiprime_uint64(uint64_t N)
{
    if (N < 4) return 0;  /* 4=2*2 is semiprime, but 0/1/2/3 are not */

    uint64_t temp = N;
    int factors = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
                if (++factors > 2) return 0;
            }
        }
    }

    if (temp > 1) {
        if (++factors > 2) return 0;
    }

    return factors == 2;
}

/* ---------- Main sequence function ---------- */
static Uint64Vec seq_cache;
static uint64_t last_checked_m = 0;

seq_t a(idx_t n)
{
    /* initialise on first call */
    static int init = 0;
    if (!init) {
        init = 1;
        init_primes();
        vec_init(&seq_cache);
    }

    if (n == 0) return 0;                 /* undefined for n=0 */

    while (seq_cache.size < (size_t)n) {
        ++last_checked_m;
        uint64_t m = last_checked_m;
        uint64_t N = 2 * m * (m + 1) + 1;   /* m^2 + (m+1)^2 */

        if (is_semiprime_uint64(N))
            vec_push(&seq_cache, m);
    }

    return (seq_t)(seq_cache.data[(size_t)n - 1]);
}
