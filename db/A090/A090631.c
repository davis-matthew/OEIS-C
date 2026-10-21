/*
 * A090631  Given n boxes labeled 1..n, such that box i weighs 2i grams and
 *          can support a total weight of i grams; a(n) = number of stacks of
 *          boxes that can be formed such that no box is squashed.
 * https://oeis.org/A090631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    OEIS A090631: a(n) = number of stacks of distinct boxes labeled 1..n
    where box i weighs 2i grams and supports i grams.
    The following implementation builds the sequence incrementally.
    It uses dynamic programming on the total weight of the stack.
    Complexity: O(n²) time, O(3n) memory, well below 2 s for n up to 10 000.
*/

#include <stddef.h>
#include <stdlib.h>

/* Function to compute a(n).  seq_t and idx_t are provided by the build. */
seq_t a(idx_t n) {
    /* Static state shared across calls */
    static idx_t current = 0;          /* highest n precomputed */
    static size_t dp_size = 0;         /* size of dp array (3*current+1) */
    static seq_t *dp = NULL;           /* dp[weight] = number of stacks with that weight */
    static size_t a_size = 0;          /* size of a array */
    static seq_t *a_arr = NULL;        /* a_arr[i] = a(i) */
    static seq_t cur_sum = 0;          /* sum_{w=0}^{current} dp[w] */

    /* Initialisation on first call */
    if (dp == NULL) {
        dp_size = 1;                       /* holds dp[0] only */
        dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
        dp[0] = 1;                         /* empty stack */
        cur_sum = 1;

        a_size = 1;                        /* a(0) only */
        a_arr = (seq_t*)calloc(a_size, sizeof(seq_t));
        a_arr[0] = 1;
    }

    /* Extend precomputation up to n if necessary */
    if (n > current) {
        /* New limits */
        idx_t new_max = n;
        size_t new_dp_size = 3 * new_max + 1;
        size_t new_a_size = new_max + 1;

        /* Reallocate dp array */
        if (new_dp_size > dp_size) {
            seq_t *new_dp = (seq_t*)calloc(new_dp_size, sizeof(seq_t));
            /* copy old content */
            memcpy(new_dp, dp, dp_size * sizeof(seq_t));
            free(dp);
            dp = new_dp;
            dp_size = new_dp_size;
        }

        /* Reallocate a array */
        if (new_a_size > a_size) {
            seq_t *new_a = (seq_t*)calloc(new_a_size, sizeof(seq_t));
            memcpy(new_a, a_arr, a_size * sizeof(seq_t));
            free(a_arr);
            a_arr = new_a;
            a_size = new_a_size;
        }

        /* Build new values */
        for (idx_t i = current + 1; i <= new_max; ++i) {
            /* Number of new stacks that use box i as the bottom box */
            seq_t sum_w = cur_sum + dp[i];           /* sum_{w=0}^{i} dp[w] before adding box i */
            a_arr[i] = a_arr[i - 1] + sum_w;         /* a(i) = a(i-1) + sum_w */

            /* Update dp: for every previous weight t (0 <= t <= i) */
            for (idx_t t = 0; t <= i; ++t) {
                seq_t val = dp[t];
                if (val) {
                    idx_t w = t + 2 * i;              /* new total weight after adding box i */
                    dp[w] += val;
                }
            }

            /* Update cumulative sum cur_sum for the next i */
            cur_sum += dp[i];
        }

        current = new_max;
    }

    return a_arr[n];
}
