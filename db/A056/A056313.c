/*
 * A056313  Number of reversible strings with n beads using exactly six
 *          different colors.
 * https://oeis.org/A056313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 360 * (S₂(n,6) + S₂(⌈n/2⌉,6))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S₂(n,k) are Stirling numbers of the second kind */

#include <stddef.h>                     /* for size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array()  */
#include "../utils/free_seq_array.h"    /* free_seq_array()   */

#define K 6

/* index into the 1‑dimensional table */
static inline idx_t pos(idx_t n, idx_t k)
{
    return n * (K + 1) + k;
}

/* sequence a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* static cache of Stirling numbers for k = 0..K */
    static seq_t *S = NULL;      /* array of size (max_n+1)*(K+1) */
    static idx_t max_n   = 0;     /* highest n for which S has been filled */

    /* grow the table if necessary */
    if (n > max_n)
    {
        /* new dimensions */
        idx_t new_max_n = n;
        size_t new_len  = (size_t)(new_max_n + 1) * (K + 1);

        /* allocate new table */
        seq_t *newS = alloc_seq_array(new_len);
        if (!newS) return 0;   /* allocation failure – return 0 as safe fallback */

        /* copy old data */
        if (S)
        {
            size_t old_len = (size_t)(max_n + 1) * (K + 1);
            for (size_t i = 0; i < old_len; ++i)
                newS[i] = S[i];
        }

        /* free the old table */
        if (S)
            free_seq_array(S);

        /* initialise row 0 : S(0,0)=1, others 0 */
        for (idx_t k = 0; k <= K; ++k)
            newS[pos(0, k)] = (k == 0) ? (seq_t)1 : (seq_t)0;

        /* compute all rows from max_n+1 up to new_max_n */
        for (idx_t i = max_n + 1; i <= new_max_n; ++i)
        {
            newS[pos(i, 0)] = (seq_t)0; /* S(i,0)=0 for i>0 */
            for (idx_t k = 1; k <= K; ++k)
            {
                seq_t term1 = (seq_t)k * newS[pos(i - 1, k)];
                seq_t term2 = newS[pos(i - 1, k - 1)];
                newS[pos(i, k)] = term1 + term2;
            }
        }

        /* replace the old table */
        S = newS;
        max_n = new_max_n;
    }

    /* ceil(n/2) */
    idx_t m = (n + 1) / 2;

    /* retrieve the required Stirling numbers */
    seq_t sn = S[pos(n, K)];      /* S₂(n,6)   */
    seq_t sm = S[pos(m, K)];      /* S₂(⌈n/2⌉,6) */

    /* a(n) = 360 * (sn + sm) */
    seq_t sum  = sn + sm;
    seq_t res  = (seq_t)360 * sum;

    return res;
}
