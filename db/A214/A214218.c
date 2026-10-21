/*
 * A214218  List of words over {1,2} with equal numbers of 1's and 2's.
 * https://oeis.org/A214218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th word over {1,2} with equal numbers of 1’s and
 *            2’s, ordered first by length (2k) and then lexicographically.
 *            The word is returned as a decimal integer (digits 1 and 2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the word length 2k such that
       sum_{i=1}^{k} C(2i,i) >= n, with k minimal. */
    seq_t cum = 0;
    idx_t k = 1;
    for (;; ++k) {
        seq_t count = binom((idx_t)(2 * k), (idx_t)k);
        if (cum + count >= (seq_t)n) break;
        cum += count;
    }

    /* Offset of n within the block of words of length 2k. */
    seq_t offset = (seq_t)n - cum - 1;
    idx_t len = 2 * k;
    idx_t ones_needed = k;
    seq_t value = 0;

    /* Unrank lexicographically to build the word. */
    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t positions_left = len - pos;
        seq_t cnt_first = 0;
        if (ones_needed > 0) {
            cnt_first = binom((idx_t)(positions_left - 1),
                              (idx_t)(ones_needed - 1));
        }
        if (offset < cnt_first) {
            /* Place digit '1'. */
            value = value * 10 + 1;
            --ones_needed;
        } else {
            /* Skip all strings that start with '1' at this position. */
            if (ones_needed > 0) offset -= cnt_first;
            /* Place digit '2'. */
            value = value * 10 + 2;
        }
    }
    return value;
}
