/*
 * A051166  Sequence is defined by property that binomial transform of
 *          (a0,a1,a2,a3,...) = (a0,a0,a0,a1,a1,a1,a2,a2,a2,a3,a3,a3,...).
 * https://oeis.org/A051166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, eigen
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 29, i64 n <= 58, i128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A051166
 *  a(0) = 1
 *  For n > 0:
 *      a(n) = a(floor(n/3)) - Σ_{k=0}^{n-1} binom(n,k) * a(k)
 *  where binom(n,k) = n!/(k!(n-k)!) is computed iteratively.
 *  This follows from the condition that the binomial transform of
 *  (a_n) equals the sequence (a_0,a_0,a_0,a_1,a_1,a_1,…) .
 */
#include <stdlib.h>

/*  Ensure the cache big enough (C++ compilers also accept this).
 *  A seq_t element is default‑constructible from 0, so we use (seq_t)0.
 */
static void ensure_cache(idx_t required, seq_t **arr, idx_t *size)
{
    if (required < *size)
        return;
    idx_t new_size = required < 4 ? 4 : required * 2;
    seq_t *new_arr = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
    if (!new_arr)  /* Out of memory: abort */
        abort();
    /* Initialise new elements to 0 */
    for (idx_t i = *size; i < new_size; ++i)
        new_arr[i] = (seq_t)0;
    *arr   = new_arr;
    *size  = new_size;
}

/*  Main function required by the system.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Grow the cache to at least n+1 elements.  */
    ensure_cache(n + 1, &cache, &cache_size);

    /* Initialise a(0)=1 if not already done.  */
    if (n == 0) {
        if (cache_size == 0 || cache[0] == (seq_t)0)
            cache[0] = (seq_t)1;
        return cache[0];
    }
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;   /* ensure a(0)=1 before further work */

    /* Compute new values up to n.  */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;   /* already computed in a previous call */
        seq_t sum   = (seq_t)0;
        seq_t coeff = (seq_t)1;          /* binom(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum += coeff * cache[k];
            if (k + 1 < i) {
                /* coeff = binom(i,k) * (i-k) / (k+1) = binom(i,k+1)  */
                coeff = coeff * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        idx_t m = i / 3;                 /* floor(i/3) */
        cache[i] = cache[m] - sum;
    }
    return cache[n];
}
