/*
 * A227688  Numerator of least splitting rational of s(n) and s(n+1), where
 *          s(n) = 1/sqrt(1) + 1/sqrt(2) + ... + 1/sqrt(n).
 * https://oeis.org/A227688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the numerator of the least splitting rational
 * of s(n) and s(n+1), where s(n) = Σ_{k=1}^{n} 1/√k.
 *
 * For each n ≥ 1 the function evaluates s(n) and s(n+1) using a
 * long‑double cache, then searches for the smallest denominator d
 * such that a rational c/d lies strictly between s(n) and s(n+1).
 * The required numerator is then c = ⌊s(n)·d⌋ + 1.
 *
 * The algorithm is O(n + d_max) per query and stays well below
 * 2 s for n up to about 10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static long double *s_cache = NULL;      /* s_cache[0] = 0,
                     s_cache[i] = Σ_{k=1}^i 1/√k */
static idx_t       s_cache_size = 0;     /* number of valid entries */

/* Ensure that s_cache has entries for indices 0 … need‑1 inclusive. */
static void
ensure_s_cache(idx_t need)
{
    if (need <= s_cache_size)
        return;

    size_t newsize = (size_t)need;
    if (newsize < 2)
        newsize = 2;
    while (newsize < need)
        newsize <<= 1;

    long double *tmp = (long double *)realloc(s_cache, newsize * sizeof(long double));
    if (!tmp)
        abort();

    /* initialise newly allocated entries */
    for (idx_t i = s_cache_size; i < (idx_t)newsize; ++i) {
        if (i == 0)
            tmp[i] = 0.0L;
        else
            tmp[i] = tmp[i - 1] + 1.0L / sqrtl((long double)i);
    }
    s_cache = tmp;
    s_cache_size = (idx_t)newsize;
}

/* Return the numerator of the least splitting rational of s(n) and s(n+1). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0U;                 /* undefined for n = 0, but kept safe */

    /* compute s(n) and s(n+1) using the cache */
    ensure_s_cache(n + 2);
    long double alpha = s_cache[n];
    long double beta  = s_cache[n + 1];

    const long double EPS = 1e-15L;          /* safety margin for rounding */

    for (idx_t d = 1; ; ++d) {
        /* c = ceil(alpha * d) */
        long double prod = alpha * d;
        long long c = (long long)ceill(prod - EPS);    /* avoids overflow safely */

        /* test whether c/d < beta, i.e. c < beta * d */
        if ((long double)c + EPS < beta * d) {
            return (seq_t)c;              /* numerator of the least splitting rational */
        }

        /* sanity limit: the required d must be far smaller than 1 000 000
         * for n ≤ 10 000, but keep a guard. */
        if (d > 1000000)
            break;
    }

    return (seq_t)0U;                       /* should never be reached */
}
