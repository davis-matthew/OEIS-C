/*
 * A024527  a(n) = sum of cubes of p(j) - p(i), for 0 <= i < j <= n, where
 *          p(0) = 1.
 * https://oeis.org/A024527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{0≤i<j≤n} (p(j)-p(i))³ where p(0)=1 and p(k) is the
 *            k‑th prime (p(1)=2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_cache = NULL;      /* primes 2,3,5,…   */
static size_t     prime_count = 0;        /* number of primes stored */
static size_t     max_n_needed = 0;       /* largest n processed */

/* Sieve of Eratosthenes up to 'limit', storing the first 'target' primes in prime_cache.
   The routine stops early when the required number of primes is obtained. */
static void sieve_primes(idx_t target, idx_t limit)
{
    /* allocate sieve buffer */
    uint8_t *comp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!comp) abort();

    size_t count = 0;
    /* sieve */
    for (size_t i = 2; i * i <= (size_t)limit; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                comp[j] = 1;

    /* collect primes */
    free(prime_cache);
    prime_cache = (uint64_t *)malloc((size_t)target * sizeof(uint64_t));
    if (!prime_cache) abort();

    for (size_t i = 2; i <= (size_t)limit && count < target; ++i)
        if (!comp[i])
            prime_cache[count++] = (uint64_t)i;

    free(comp);
    prime_count = count;
    /* prime_count must be equal to target for our usage */
}

/* Ensure that we have stored at least the first 'n' primes. */
static void ensure_primes(idx_t n)
{
    if (n <= max_n_needed) return;   /* already available */

    /* Rough upper bound for the nth prime: n*20 is enough for n≤10000 */
    idx_t estimate = n * 20;
    if (estimate < 100) estimate = 100;

    sieve_primes((size_t)n, estimate);
    max_n_needed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0, safe fallback */

    ensure_primes((size_t)n);            /* prime_cache[now contains first n primes) */

    seq_t result = 0;
    seq_t S1 = 0, S2 = 0, S3 = 0;       /* prefix sums over t_i, t_i², t_i³ */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t t_j;
        if (j == 0)
            t_j = (seq_t)1;                  /* p(0) = 1 */
        else
            t_j = (seq_t)prime_cache[(size_t)(j - 1)];

        if (j > 0) {
            seq_t t_j2 = t_j * t_j;
            seq_t t_j3 = t_j2 * t_j;
            seq_t j_t  = (seq_t)j;

            /* (t_j - t_i)^3 summed over i<j */
            result += j_t * t_j3
                      - (seq_t)3 * t_j2 * S1
                      + (seq_t)3 * t_j  * S2
                      - S3;
        }

        /* update prefix sums for next j */
        seq_t t_j2 = t_j * t_j;
        seq_t t_j3 = t_j2 * t_j;
        S1 += t_j;
        S2 += t_j2;
        S3 += t_j3;
    }

    return result;
}
