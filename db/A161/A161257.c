/*
 * A161257  Number of partitions of n into central polygonal numbers A000124
 *          where every part appears at least 4 times.
 * https://oeis.org/A161257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into central polygonal numbers
 *            (A000124: 1, 2, 4, 7, 11, …) where each distinct part appears at
 *            least four times. The implementation uses dynamic programming
 *            with a prefix‑sum technique to handle the “at least 4”
 *            restriction efficiently
 * Range:     u8 n <= 67, u16 n <= 209, u32 n <= 847, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* count partitions of n with each used part ≥ 4 times */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safety for indices below the offset */

    /* static state for memoisation across calls */
    static idx_t        computed_max = 0;
    static seq_t        *dp          = NULL;   /* dp[i] = a(i) */
    static seq_t        *prefix      = NULL;   /* temporary prefix sums */
    static idx_t        dp_size      = 0;

    if (n > computed_max) {
        /* free old arrays if they exist */
        if (dp)          free_seq_array(dp);
        if (prefix)      free_seq_array(prefix);

        /* allocate new arrays up to the required size */
        dp_size = n + 1;
        dp      = alloc_seq_array(dp_size);
        prefix  = alloc_seq_array(dp_size);
        if (!dp || !prefix)            /* allocation failure */
            return 0;

        /* initialise dp */
        for (idx_t i = 0; i < dp_size; ++i) dp[i] = 0;
        dp[0] = 1;                     /* one way to form 0 (empty sum) */

        /* iterate over all central polygonal numbers that do not exceed n */
        for (idx_t k = 1;; ++k) {

            /* central polygonal number: (k-1)*k/2 + 1  (fits in seq_t) */
            seq_t T = ((seq_t)(k - 1) * (seq_t)k) / 2 + 1;
            if ((idx_t)T > n) break;         /* all larger coins are useless */

            /* copy current dp into an auxiliary array 'old' */
            seq_t *old = alloc_seq_array(dp_size);
            if (!old) {                      /* allocation failure */
                free_seq_array(dp);
                free_seq_array(prefix);
                return 0;
            }
            for (idx_t i = 0; i < dp_size; ++i) old[i] = dp[i];

            /* for each residue modulo T, compute prefix sums and update dp */
            for (idx_t r = 0; r < (idx_t)T; ++r) {
                idx_t max_q = (n - r) / (idx_t)T;   /* largest k with r + k*T ≤ n */

                /* prefix sums of old over the arithmetic progression r, r+T, … */
                idx_t idx = r;
                seq_t sum = 0;
                for (idx_t q = 0; q <= max_q; ++q, idx += (idx_t)T) {
                    sum += old[idx];
                    prefix[q] = sum;
                }

                /* write new dp values using the “≥ 4” restriction */
                idx = r;
                for (idx_t q = 0; q <= max_q; ++q, idx += (idx_t)T) {
                    seq_t val = old[idx];           /* using 0 copies */
                    if (q >= 4)                     /* at least 4 copies */
                        val += prefix[q - 4];
                    dp[idx] = val;
                }
            }

            free_seq_array(old);
        }

        computed_max = n;
    }

    return dp[n];
}
