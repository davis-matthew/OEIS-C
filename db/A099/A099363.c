/*
 * A099363  An inverse Chebyshev transform of 1-x.
 * https://oeis.org/A099363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n * Catalan(floor((n+1)/2)) The Catalan numbers
 *            satisfy C_0 = 1, C_k = C_{k-1} * (4*k - 2) / (k + 1) for k ≥ 1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for Catalan numbers.  The array grows on demand. */
static seq_t *catalan_cache = NULL;
static idx_t  catalan_cache_len = 0;

/* Compute the k‑th Catalan number, expanding the cache if necessary. */
static seq_t catalan(idx_t k)
{
    if (k >= catalan_cache_len)
    {
        /* Allocate or enlarge the cache to hold indices up to k. */
        seq_t *tmp = (seq_t *)realloc(catalan_cache,
                                      sizeof(seq_t) * (k + 1));
        if (!tmp)
            return 0;                      /* allocation failure – return 0 */
        catalan_cache = tmp;

        if (catalan_cache_len == 0)        /* initialise first element */
            catalan_cache[0] = 1;

        /* Fill entries that have not yet been computed. */
        for (idx_t i = catalan_cache_len; i <= k; ++i)
        {
            if (i == 0)
            {
                catalan_cache[0] = 1;     /* already set, but keep for safety */
            }
            else
            {
                /* C_i = C_{i-1} * (4*i - 2) / (i + 1)  */
                seq_t num = catalan_cache[i - 1];
                num *= (seq_t)(4 * i - 2);
                catalan_cache[i] = num / (seq_t)(i + 1);
            }
        }
        catalan_cache_len = k + 1;
    }
    return catalan_cache[k];
}

/* Public entry point required by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1)  /* defensive guard against underflow, should not happen */
        return 0;

    idx_t k = (n + 1) / 2;            /* floor((n+1)/2) */
    seq_t val = catalan(k);           /* Catalan number */
    if (n & 1)                        /* odd n → negative sign */
        val = -val;
    return val;
}
