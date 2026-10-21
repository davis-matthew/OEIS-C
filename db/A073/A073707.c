/*
 * A073707  Coefficients of a power series whose convolution consists of only
 *          the even-indexed terms of the sequence.
 * https://oeis.org/A073707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 163, u64 n <= 547,
 *            u128 n <= 1955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073707:  a(0)=1;  for m≥0   a(2m)=∑_{i=0}^{m} a(i)·a(m−i) ,  a(2m+1)=a(2m)
    The sequence satisfies the functional equation A(x)= (1+x)·A(x²)².
    The implementation builds terms iteratively, storing all values
    up to the largest n requested.  This runs in O(n²) time, which is
    fast enough for n up to about 10 000 on the typical test machine. */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* array of computed values  */
static idx_t  cache_size = 0;     /* number of elements currently stored */

/* Return the nth term of A073707.  The function may be called with
   arbitrary values of n and in any order; previously computed terms
   are memoised.  The cache is grown only when necessary. */
seq_t a(idx_t n)
{
    /* If the requested value is already cached, return it immediately. */
    if (n < cache_size) {
        return a_cache[(size_t)n];
    }

    /* Grow the cache to hold indices 0..n.  Use realloc to preserve
       any existing data.  On allocation failure we return 0 as a
       graceful fallback. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        return (seq_t)0;          /* memory shortage – give up */
    }
    a_cache = tmp;

    /* Initialise the very first two terms if this is the first run. */
    if (cache_size == 0) {
        a_cache[0] = (seq_t)1;    /* a(0)=1 */
    }
    /* Ensure a(1) exists before it is needed. */
    if (cache_size <= 1 && new_size > 1) {
        a_cache[1] = a_cache[0];
    }

    /* Compute any new terms up to n in increasing order.  For each
       even index k we perform the convolution sum; odd indices simply
       copy the previous even value. */
    for (idx_t k = cache_size; k <= n; ++k) {
        if ((k & 1) == 0) {                 /* even k = 2m */
            idx_t m = k >> 1;
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= m; ++i) {
                sum += a_cache[(size_t)i] * a_cache[(size_t)(m - i)];
            }
            a_cache[(size_t)k] = sum;
        } else {                             /* odd k = 2m+1  */
            a_cache[(size_t)k] = a_cache[(size_t)(k - 1)];
        }
    }

    cache_size = new_size;                  /* updated cache size   */
    return a_cache[(size_t)n];
}
