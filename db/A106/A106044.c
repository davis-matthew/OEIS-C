/*
 * A106044  Difference between n-th prime and next larger perfect square.
 * https://oeis.org/A106044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = (⌊√p_n⌋ + 1)² – p_n, where p_n is the n‑th prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS A106044.  */
#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;       /* number of primes stored */
static size_t   prime_cap   = 0;       /* current capacity of prime_cache */
static uint64_t next_check  = 1;       /* next candidate to test for primality */

/* Integer square‑root: floor(sqrt(n)) using 128‑bit arithmetic. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32) - 1;   /* sqrt(2^64‑1) < 2^32 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Ensure that prime_cache contains at least 'n' primes. */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt) return;

    if (prime_cache == NULL) {
        prime_cap = 128;
        prime_cache = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) abort();
    }

    while (prime_cnt < (size_t)n) {
        uint64_t cand = ++next_check;
        int isprime = 1;
        uint64_t limit = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt && prime_cache[i] <= limit; ++i) {
            if (cand % prime_cache[i] == 0) { isprime = 0; break; }
        }
        if (isprime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = cand;
        }
    }
}

/* OEIS A106044 – difference between the n‑th prime and the next larger perfect square. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; safety for n<1 */

    ensure_primes(n);

    uint64_t p = prime_cache[(size_t)(n - 1)];
    uint64_t s = isqrt_u64(p);
    uint64_t k = s + 1;

    seq_t kk   = (seq_t)k;
    seq_t kkm  = kk * kk;           /* (k+1)²   */
    seq_t pp   = (seq_t)p;
    seq_t diff = kkm - pp;          /* difference in seq_t */

    return diff;
}
