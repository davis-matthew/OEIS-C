/*
 * A080428  First differences of A079255.
 * https://oeis.org/A080428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  start with the single digit 3.
   For each iteration every 2 is replaced by 33 and every 3 is replaced by 233,
   concatenating the results.  The infinite concatenation of all iterations
   gives the sequence a(n).  This implementation generates the iteration
   that contains the requested position and returns the element at that
   position.  It is O(number of elements up to n), fast enough for n ≤ 10 000. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Find which iteration contains the nth element. */
    idx_t t = 1;          /* number of 3's in current iteration */
    idx_t s = 0;          /* number of 2's in current iteration */
    idx_t len = 1;        /* length of current iteration */
    idx_t cum = 1;        /* cumulative length up to current iteration */
    idx_t cum_prev = 0;   /* cumulative length before this iteration */
    idx_t iter = 0;       /* number of iterations performed */

    while (cum < n) {
        idx_t t_next = 2 * t + 2 * s; /* 3 → 233 gives two 3's, 2 → 33 gives two 3's */
        idx_t s_next = t;            /* 3 → 233 gives one 2 */
        len = t_next + s_next;
        cum_prev = cum;
        cum += len;
        t = t_next;
        s = s_next;
        ++iter;
    }

    /* Index of the desired element inside the found iteration (0‑based). */
    idx_t offset = n - cum_prev - 1;

    /* Build the sequence up to this iteration. */
    seq_t *cur = alloc_seq_array(1);
    cur[0] = (seq_t)3;            /* S₀ = [3] */
    size_t cur_len = 1;

    for (idx_t k = 0; k < iter; ++k) {
        /* Count how many 2's and 3's are present. */
        size_t cnt2 = 0, cnt3 = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == (seq_t)2) {
                ++cnt2;
            } else {
                /* The only other possible value is 3. */
                ++cnt3;
            }
        }

        /* Compute length of the next iteration. */
        size_t next_len = 2 * cnt2 + 3 * cnt3;
        seq_t *next = alloc_seq_array(next_len);
        size_t pos = 0;

        /* Apply the substitution rule to build the next iteration. */
        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == (seq_t)2) {
                next[pos++] = (seq_t)3;
                next[pos++] = (seq_t)3;
            } else { /* cur[i] == 3 */
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)3;
                next[pos++] = (seq_t)3;
            }
        }

        free_seq_array(cur);
        cur = next;
        cur_len = next_len;
    }

    seq_t result = cur[offset];
    free_seq_array(cur);
    return result;
}
