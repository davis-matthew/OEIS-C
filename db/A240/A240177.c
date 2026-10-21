/*
 * A240177  Number of partitions of n such that (least part) >= (multiplicity
 *          of least part).
 * https://oeis.org/A240177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n such that the least part
   is greater than or equal to its multiplicity (OEIS A240177). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The empty partition is counted for n = 0. */
    if (n == 0) return (seq_t)1;

    size_t len = (size_t)(n + 1);

    /* dp_next holds P(s, min+1), the number of partitions of s
       using parts all at least min+1.  Initially only the empty
       partition exists for sum 0. */
    seq_t *dp_next = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp_next) return (seq_t)0;          /* out of memory */
    dp_next[0] = (seq_t)1;

    /* dp_cur will hold P(s, min).  It will be filled on each
       iteration; no need to initialise it beforehand. */
    seq_t *dp_cur  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp_cur) { free(dp_next); return (seq_t)0; }

    seq_t ans = (seq_t)0;

    /* Iterate over the possible minimum part values.
       min ranges from n+1 down to 2 so that the corresponding
       least part l = min-1 ranges from n down to 1. */
    for (idx_t min = n + 1; min >= 2; --min)
    {
        /* Build dp_cur[s] = dp_next[s] + dp_cur[s - min] (if s>=min). */
        for (idx_t s = 0; s <= n; ++s)
        {
            seq_t val = dp_next[s];
            if (s >= min) val += dp_cur[s - min];
            dp_cur[s] = val;
        }

        /* Count partitions whose smallest part is l = min-1. */
        idx_t l = min - 1;

        /* The multiplicity m of the least part must satisfy
           l >= m and l*m <= n.  Thus m ranges from 1 to max_m. */
        idx_t m_limit = n / l;          /* l*m cannot exceed n */
        idx_t max_m = l;
        if (m_limit < max_m) max_m = m_limit;

        for (idx_t m = 1; m <= max_m; ++m)
        {
            idx_t s = n - l * m;
            ans += dp_cur[s];
        }

        /* Prepare for the next iteration: dp_next becomes dp_cur.
           The old dp_next becomes the new dp_cur and will be
           overwritten in the next iteration. */
        seq_t *tmp = dp_next;
        dp_next   = dp_cur;
        dp_cur    = tmp;
    }

    free(dp_next);
    free(dp_cur);

    return ans;
}
