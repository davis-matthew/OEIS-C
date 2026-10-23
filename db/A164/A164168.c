/*
 * A164168  Number of binary strings of length n with equal numbers of 0010
 *          and 0110 substrings.
 * https://oeis.org/A164168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   counts binary strings of length n in which the number of
 *            substrings 0010 equals the number of substrings 0110. The
 *            algorithm is a dynamic programming over the last three bits and
 *            the current difference between the two substring counts
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return 1;

    /* The maximum possible absolute difference of counts is at most n-3.
       We allocate arrays that can hold up to |diff| <= n. */
    idx_t maxDiff = n;         /* shift for indexing diff in [−n, n] */
    idx_t size   = 2 * maxDiff + 1;
    idx_t shift  = maxDiff;    /* index corresponding to diff = 0 */

    /* Allocate state tables for current and next length */
    seq_t *dp_cur[8];
    seq_t *dp_next[8];
    for (int s = 0; s < 8; ++s) {
        dp_cur[s]  = (seq_t *)calloc(size, sizeof(seq_t));
        dp_next[s] = (seq_t *)calloc(size, sizeof(seq_t));
    }
    /* Initial state: empty string, suffix 000, diff = 0 */
    dp_cur[0][shift] = 1;

    /* Iterate over length of the string */
    for (idx_t len = 0; len < n; ++len) {
        /* Range of possible differences at current length */
        idx_t diff_min = (len >= 3) ? -(idx_t)(len - 3) : 0;
        idx_t diff_max = (len >= 3) ?   (idx_t)(len - 3) : 0;
        idx_t cur_min  = shift + diff_min;
        idx_t cur_max  = shift + diff_max;

        /* Range for the next length */
        idx_t new_diff_min = (len + 1 >= 3) ? -(idx_t)(len + 1 - 3) : 0;
        idx_t new_diff_max = (len + 1 >= 3) ?   (idx_t)(len + 1 - 3) : 0;
        idx_t next_min = shift + new_diff_min;
        idx_t next_max = shift + new_diff_max;
        idx_t next_len = next_max - next_min + 1;

        /* Clear the next state tables within the used range */
        for (int s = 0; s < 8; ++s) {
            memset(dp_next[s] + next_min, 0,
                   (size_t)next_len * sizeof(seq_t));
        }

        /* Transition */
        for (int s = 0; s < 8; ++s) {
            for (idx_t d = cur_min; d <= cur_max; ++d) {
                seq_t val = dp_cur[s][d];
                if (!val) continue;
                for (int bit = 0; bit <= 1; ++bit) {
                    int new_s = ((s << 1) & 7) | bit;
                    idx_t diff_index = d;
                    if (len >= 3) {
                        int window = ((s << 1) | bit) & 0xF;
                        if (window == 0b0010)          diff_index += 1;
                        else if (window == 0b0110)     diff_index -= 1;
                    }
                    dp_next[new_s][diff_index] += val;
                }
            }
        }

        /* Swap current and next */
        for (int s = 0; s < 8; ++s) {
            seq_t *tmp = dp_cur[s];
            dp_cur[s]  = dp_next[s];
            dp_next[s] = tmp;
        }
    }

    /* Sum over all suffixes for difference zero */
    seq_t result = 0;
    for (int s = 0; s < 8; ++s)
        result += dp_cur[s][shift];

    /* Free allocated memory */
    for (int s = 0; s < 8; ++s) {
        free(dp_cur[s]);
        free(dp_next[s]);
    }
    return result;
}
