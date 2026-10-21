/*
 * A226739  Row 4 of array in A226513.
 * https://oeis.org/A226739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) – 2*∑_{k=1}^{n-1} (-1)^k * binom(n-1,k) *
 *            a(n-k), with a(0)=1. This uses integer arithmetic only and
 *            avoids overflow for the values that fit in seq_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated size */
    static idx_t  sz    = 0;   /* number of entries computed */

    /* create initial cache if needed */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 */
        cap   = 1;
        sz    = 0;
    }

    /* enlarge cache as necessary */
    while (cap <= n) {
        idx_t new_cap = cap ? cap << 1 : 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    while (sz <= n) {
        idx_t m = sz;               /* index to compute */

        if (m == 0) {               /* a(0) = 1 */
            cache[0] = (seq_t)1;
            sz++;
            continue;
        }

        /* Compute the sums that appear in the recurrence.  We keep
           separate sums for even and odd k so that we never need to
           represent negative intermediate values when seq_t is unsigned. */
        seq_t pos_sum = 0, neg_sum = 0;
        idx_t row = m - 1;          /* we need binom(row, k) for k=1..row */

        seq_t comb = 1;             /* C(row,0) */
        for (idx_t k = 1; k <= row; ++k) {
            /* update comb to C(row, k) */
            comb = comb * (seq_t)(row - k + 1) / (seq_t)k;

            /* term = C(row,k) * a(m-k) */
            seq_t term = comb * cache[m - k];

            if ((k & 1) == 0) {     /* even k → positive contribution */
                pos_sum += term;
            } else {                /* odd k  → negative contribution */
                neg_sum += term;
            }
        }

        /* a(m) = 5*a(m-1) – 2*pos_sum + 2*neg_sum  (no unsigned underflow) */
        seq_t temp = (seq_t)5 * cache[m - 1];
        temp += (seq_t)2 * neg_sum;
        cache[m] = temp - (seq_t)2 * pos_sum;

        sz++;
    }

    return cache[n];
}
