/*
 * A161238  Number of partitions of n into Catalan numbers A000108 where every
 *          part appears at least 13 times.
 * https://oeis.org/A161238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 616, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, catalan, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161238
 * Number of partitions of n into Catalan numbers A000108
 * where every part appears at least 13 times.
 *
 * The algorithm uses dynamic programming over the set of distinct
 * Catalan numbers ≤ n.  For each Catalan number c, we treat it as
 * a coin that may be used either 0 times or at least 13 times.
 * The contribution of c is computed efficiently by sliding‐window
 * sums over each residue class modulo c.
 *
 * Complexity:  O(n·C)   (C ≈ number of distinct Catalan numbers ≤ n,
 *                      which is < 10 for n ≤ 10000).
 * Memory usage:  O(n)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/catalan.h"              /* catalan(k) */
#include "../utils/alloc_seq_array.h"      /* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"      /* free_seq_array(seq_t *) */

seq_t a(idx_t n)
{
    /* Prepare a list of distinct Catalan numbers ≤ n.
       Catalan(0) = 1 and Catalan(1) = 1; we keep only one occurrence. */
    seq_t cat_vals[32];
    size_t cat_cnt = 0;
    seq_t last_c = 0;

    for (idx_t k = 0; ; ++k) {
        seq_t c = catalan(k);
        if (c > (seq_t)n) break;
        if (k > 0 && c == last_c) continue;   /* skip Duplicate 1 */
        cat_vals[cat_cnt++] = c;
        last_c = c;
    }

    size_t N = (size_t)n;
    size_t arr_size = N + 1;

    /* Allocate DP arrays and initialise to zero. */
    seq_t *dp_old = alloc_seq_array(arr_size);
    seq_t *dp_new = alloc_seq_array(arr_size);
    if (!dp_old || !dp_new) {
        free_seq_array(dp_old);
        free_seq_array(dp_new);
        return 0;
    }
    memset(dp_old, 0, arr_size * sizeof(seq_t));
    dp_old[0] = (seq_t)1;   /* empty partition */

    /* Process each Catalan number as a coin. */
    for (size_t ci = 0; ci < cat_cnt; ++ci) {
        idx_t c = (idx_t)cat_vals[ci];

        /* Start dp_new as a copy of dp_old (zero copies case). */
        memcpy(dp_new, dp_old, arr_size * sizeof(seq_t));

        /* For each residue r modulo c, update contributions. */
        for (idx_t r = 0; r < c; ++r) {
            seq_t sum0   = (seq_t)0;     /* cumulative sum of dp_old over this residue */
            seq_t win_sum = (seq_t)0;    /* sum of the last 12 values (window) */
            seq_t win[12] = {0};
            size_t win_fill = 0;
            size_t win_idx  = 0;

            for (idx_t m = 0; ; ++m) {
                idx_t s_idx = r + m * c;
                if (s_idx > n) break;
                size_t s = (size_t)s_idx;

                seq_t vec_val = dp_old[s];   /* dp_old value at this sum */

                if (m >= 13) {
                    seq_t cont = sum0 - win_sum;     /* contributions with ≥13 copies */
                    dp_new[s] = dp_new[s] + cont;
                }

                /* Update running sums. */
                sum0 = sum0 + vec_val;

                if (win_fill < 12) {
                    win_sum = win_sum + vec_val;
                    win[win_idx] = vec_val;
                    win_idx = (win_idx + 1U) % 12U;
                    ++win_fill;
                } else {
                    seq_t old = win[win_idx];
                    win_sum = win_sum + vec_val - old;
                    win[win_idx] = vec_val;
                    win_idx = (win_idx + 1U) % 12U;
                }
            }
        }

        /* Prepare for next coin: swap pointers. */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    seq_t ans = dp_old[(size_t)n];

    free_seq_array(dp_old);
    free_seq_array(dp_new);

    return ans;
}
