/*
 * A010818  Expansion of Product (1 - x^k)^10 in powers of x.
 * https://oeis.org/A010818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>0, a(n)=-(10/n) * Σ_{k=1..n} sigma(k) * a(n‑k)
 *            where sigma(k) is the sum of divisors function
 * Range:     i8 n <= 4, i16 n <= 34, i32 n <= 1653, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static caches for computed values and sigma(k). */
    static seq_t *a_cache   = NULL;
    static size_t   a_len   = 0;          /* first unused index in a_cache   */
    static seq_t *sigma_cache = NULL;
    static size_t   sigma_len  = 0;       /* first unused index in sigma_cache */

    size_t need = (size_t)n + 1;          /* we must compute up to index n */

    /* Grow / compute sigma_cache if necessary. */
    if (sigma_len < need)
    {
        seq_t *new_sigma = (seq_t *)calloc(need, sizeof(seq_t));
        if (!new_sigma)
            return (seq_t)0;              /* out‑of‑memory, should not happen */

        /* Classic divisor–sums sieve: for each i add i to all multiples. */
        for (size_t i = 1; i < need; ++i)
        {
            for (size_t j = i; j < need; j += i)
            {
                new_sigma[j] += (seq_t)i;
            }
        }

        free(sigma_cache);
        sigma_cache = new_sigma;
        sigma_len   = need;
    }

    /* Grow a_cache to hold values up to n. */
    if (a_len < need)
    {
        seq_t *new_a = (seq_t *)realloc(a_cache, need * sizeof(seq_t));
        if (!new_a)
            return (seq_t)0;              /* out‑of‑memory */
        a_cache = new_a;

        /* Initialise the very first term if not done yet. */
        if (a_len == 0)
        {
            a_cache[0] = 1;
            a_len      = 1;
        }

        /* Compute new terms from a_len to need-1 (inclusive). */
        for (size_t k = a_len; k < need; ++k)
        {
            seq_t s = 0;
            for (size_t j = 1; j <= k; ++j)
            {
                s += sigma_cache[j] * a_cache[k - j];
            }
            /* Division is exact by the definition of the recurrence. */
            a_cache[k] = -((seq_t)10 * s) / (seq_t)k;
        }
        a_len = need;
    }

    return a_cache[n];
}
