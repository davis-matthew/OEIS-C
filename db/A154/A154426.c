/*
 * A154426  a(n) = n + (sum of preceding terms) mod n; a(0) = 0.
 * https://oeis.org/A154426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = n + (sum_{k=0}^{n-1} a(k)) mod n (n > 0)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 6 the recurrence simplifies to a(n) = 2*(n-1).   */
/* The implementation keeps a memoised cache that grows on demand. */

#include <stdlib.h>

static seq_t *cache = NULL;    /* a(n) for n = 0 .. top-1           */
static seq_t *sums  = NULL;    /* sums[n] = Σ_{k=0}^{n-1} a(k)      */
static size_t top    = 0;      /* number of entries currently stored */

/* Return a(n) for any n ≥ 0, calls may be in any order. */
seq_t a(idx_t n)
{
    /* Ensure the cache has room for n */
    if ((size_t)n >= top) {
        size_t newsize = (size_t)n + 1;
        /* Reallocate a cache for a(n) values */
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        /* Reallocate a cache for prefix sums */
        sums  = (seq_t*)realloc(sums,  newsize * sizeof(seq_t));

        /* Compute entries from the previous top up to n */
        for (size_t i = top; i <= (size_t)n; ++i) {
            seq_t an;
            if (i == 0) {
                an = 0;                 /* a(0)=0 */
            } else if (i <= 6) {
                seq_t sum_prev = sums[i - 1];
                an = (seq_t)i + (sum_prev % (seq_t)i);
            } else {
                an = 2 * ((seq_t)i - 1);
            }
            cache[i] = an;
            /* sums[i] = sum_{k=0}^{i-1} a(k) */
            if (i == 0) {
                sums[i] = 0;
            } else {
                sums[i] = sums[i - 1] + an;
            }
        }
        top = newsize;
    }
    return cache[(size_t)n];
}
