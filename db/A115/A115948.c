/*
 * A115948  a(n) = (2^(semiprime(n)-1)) modulo (semiprime(n)^2).
 * https://oeis.org/A115948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(semiprime(n)-1)) modulo (semiprime(n)^2)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Fast modular exponentiation for 64‑bit operands (fits in seq_t).    */
/* Returns the result as seq_t. The cast relies on the building system */
static seq_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

/* ------------------------------------------------------------------ */
/* Precomputation of a list of semiprimes (numbers with exactly two  */
/* prime factors, counted with multiplicity).                           */
#define PRECOMP_LIMIT 400000     /* large enough for n up to ~1e4 */

static idx_t *semiprimes = NULL;    /* array of semiprimes (type idx_t) */
static size_t semi_len   = 0;        /* number of stored semiprimes      */
static int    initialized = 0;       /* flag to ensure one‑time init      */

static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void init_semiprimes(void)
{
    if (initialized) return;
    initialized = 1;

    /* 1. Sieve primes up to PRECOMP_LIMIT */
    size_t limit = PRECOMP_LIMIT;
    uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_prime) return;
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    size_t primes_cap = 40000;
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_len = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(primes); free(is_prime); return; }
                primes = tmp;
            }
            primes[primes_len++] = (uint32_t)i;
        }
    free(is_prime);

    /* 2. Produce all semiprimes <= PRECOMP_LIMIT */
    size_t semi_cap = 50000;
    idx_t *tmp = (idx_t *)malloc(semi_cap * sizeof(idx_t));
    size_t count = 0;

    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        for (size_t j = i; j < primes_len; ++j) { /* include p >= q to avoid duplicates */
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > limit) break;
            if (count == semi_cap) {
                semi_cap *= 2;
                idx_t *tmp2 = (idx_t *)realloc(tmp, semi_cap * sizeof(idx_t));
                if (!tmp2) { free(tmp); free(primes); return; }
                tmp = tmp2;
            }
            tmp[count++] = (idx_t)prod;
        }
    }
    free(primes);

    /* 3. Sort the list */
    qsort(tmp, count, sizeof(idx_t), cmp_idx_t);

    /* 4. Store result */
    semiprimes = tmp;
    semi_len   = count;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined for n < 1 */
    init_semiprimes();
    if (n > semi_len) return 0;           /* beyond pre‑computation */
    idx_t p = semiprimes[n - 1];           /* nth semiprime (1‑based) */
    uint64_t mod = (uint64_t)p * (uint64_t)p;       /* p^2 fits 64‑bit */
    uint64_t exp = (uint64_t)p - 1;                     /* p-1        */
    return modpow_uint64(2, exp, mod);
}
