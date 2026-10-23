/*
 * A195588  a(n) = Sum_{k=0..2*n} (-1)^k * moebius(2*n-k+1) * moebius(k+1).
 * https://oeis.org/A195588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..2*n} (-1)^k * mu(2*n-k+1) * mu(k+1)
 * Range:     i8 n <= 943, i16 n <= 1001, i32 n <= 1001, i64 n <= 1001,
 *            i128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Cached Mobius values. mu_cache[i] = μ(i)  for 0 <= i <= mu_cache_max. */
static int8_t *mu_cache = NULL;
static idx_t mu_cache_max = 0;

/* Ensure that mu_cache contains values up to `limit` (inclusive). */
static void ensure_mu(idx_t limit)
{
    if (limit <= mu_cache_max)
        return;

    /* Allocate a new cache large enough for all required values. */
    int8_t *new_cache = (int8_t *)malloc((limit + 1) * sizeof(int8_t));
    if (!new_cache)
        exit(EXIT_FAILURE);

    /* Linear sieve for the smallest prime factor (spf). */
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) {
        free(new_cache);
        exit(EXIT_FAILURE);
    }
    memset(spf, 0, (limit + 1) * sizeof(idx_t));

    idx_t primes_cap = limit / 10 + 10;            /* rough upper bound */
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) {
        free(spf);
        free(new_cache);
        exit(EXIT_FAILURE);
    }
    idx_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < primes_cap)
                primes[primes_cnt++] = i;
        }
        for (idx_t j = 0; j < primes_cnt &&
                         primes[j] <= spf[i] &&
                         i * primes[j] <= limit; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* Compute μ from the spf array. */
    new_cache[0] = 0;          /* unused */
    new_cache[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t x = i;
        int cnt = 0;          /* distinct prime factors */
        int square_free = 1;  /* flag */

        while (x > 1) {
            idx_t p = spf[x];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
                if (e > 1) {
                    square_free = 0;
                    break;
                }
            }
            if (!square_free)
                break;
            ++cnt;
        }
        new_cache[i] = square_free ? ((cnt & 1) ? -1 : 1) : 0;
    }

    free(spf);
    free(primes);

    /* Replace the old cache.  Ownership of the old buffer is
       transferred to free() below. */
    free(mu_cache);
    mu_cache = new_cache;
    mu_cache_max = limit;
}

seq_t a(idx_t n)
{
    idx_t limit = 2 * n + 1;        /* maximum argument needed for μ */
    ensure_mu(limit);

    seq_t sum = 0;
    for (idx_t m = 1; m <= limit; ++m) {
        int8_t mu1 = mu_cache[m];
        int8_t mu2 = mu_cache[2 * n - m + 2]; /* μ(2n - (m-1) + 1) */

        /* sign = (-1)^(m-1);  +1 when m is odd, -1 when m is even. */
        seq_t sign = (m & 1) ? 1 : -1;

        sum += sign * (seq_t)mu1 * (seq_t)mu2;
    }
    return sum;
}
