/*
 * A106878  Primes of the form 3x^2+xy+3y^2.
 * https://oeis.org/A106878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the :primes of the form 3x^2 + x*y + 3y^2 (OEIS A106878).
   The function returns the n‑th prime of this form (n ≥ 1).  */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* ---------- small prime sieve (deterministic trial division) ---------- */
#define SMALL_PRIME_LIMIT 200000

static uint32_t *small_primes = NULL;
static size_t small_cnt = 0;
static int small_inited = 0;

static void init_small_primes(void)
{
    if (small_inited) return;
    small_inited = 1;

    small_primes = (uint32_t *)malloc(SMALL_PRIME_LIMIT * sizeof(uint32_t));
    assert(small_primes);

    uint8_t *sieve = (uint8_t *)calloc(SMALL_PRIME_LIMIT + 1, sizeof(uint8_t));
    assert(sieve);

    for (size_t i = 2; i <= (size_t)SMALL_PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            small_primes[small_cnt++] = (uint32_t)i;
            if (i * i <= (size_t)SMALL_PRIME_LIMIT) {
                for (size_t j = i * i; j <= (size_t)SMALL_PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* ---------- simple primality test (deterministic Miller–Rabin / trial) ---------- */
static int is_prime64(uint64_t n)
{
    if (n < 2) return 0;
    init_small_primes();
    for (size_t i = 0; i < small_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return n == p;
    }
    return 1;
}

/* ---------- integer square root (64‑bit) ---------- */
static uint64_t uint64_sqrt(uint64_t n)
{
    uint64_t r = (uint64_t)(sqrt((long double)n) + 0.5L);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* ---------- check whether p = 3x^2 + x*y + 3y^2 for some integers x,y ---------- */
static int representable(uint64_t p)
{
    if (p == 0) return 0;
    uint64_t max_y = uint64_sqrt(p / 3ULL);
    for (int64_t y = -(int64_t)max_y; y <= (int64_t)max_y; ++y) {
        int64_t y2 = y * y;
        __int128 D = -( (__int128)35 * y2 ) + (__int128)12 * p;
        if (D < 0) continue;
        uint64_t D_u = (uint64_t)D;
        uint64_t sq = uint64_sqrt(D_u);
        if (sq * sq != D_u) continue;
        int64_t num1 = -y + (int64_t)sq;
        if (num1 % 6 == 0) return 1;
        int64_t num2 = -y - (int64_t)sq;
        if (num2 % 6 == 0) return 1;
    }
    return 0;
}

/* ---------- dynamic array helpers ----------
 * `arr` holds seq_t elements; capacity is doubled when needed.  */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = *cap ? (*cap << 1) : 64;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    assert(tmp);
    *arr = tmp;
    *cap = newcap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static cache of found primes of the required form */
    static seq_t *primes = NULL;
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;
    static uint64_t last_checked = 2;

    if (n == 0) return 0;          /* undefined; safe fallback */

    /* extend cache until we have at least n terms */
    if (prime_cnt < (size_t)n) {
        if (!primes) {
            primes = (seq_t *)malloc(64 * sizeof(seq_t));
            assert(primes);
            prime_cap = 64;
        }
        while (prime_cnt < (size_t)n) {
            for (uint64_t cand = last_checked; /* infinite loop */; ++cand) {
                if (!is_prime64(cand)) continue;
                if (!representable(cand)) continue;
                /* store the prime */
                ensure_capacity(&primes, &prime_cap, prime_cnt + 1);
                primes[prime_cnt++] = (seq_t)cand;
                last_checked = cand + 1;
                break;
            }
        }
    }
    return primes[n - 1];
}
