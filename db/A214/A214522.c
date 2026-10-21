/*
 * A214522  List of words over {1,5} with equal numbers of 1's and 5's.
 * https://oeis.org/A214522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th smallest integer over {1,5} having an equal
 *            number of 1’s and 5’s (numeric order)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the length 2k of the desired number. */
    seq_t cumulative = 0;
    idx_t k = 1;
    seq_t count;
    while (1) {
        count = binom((idx_t)(2*k), (idx_t)k);
        cumulative += count;
        if (cumulative >= n) break;
        k++;
    }

    /* Compute zero‑based offset within the block of length 2k. */
    seq_t prev_cumulative = cumulative - count;
    seq_t target = (seq_t)n;
    seq_t offset = target - prev_cumulative - 1;

    /* Build the number digit by digit. */
    seq_t result = 0;
    idx_t rem = 2*k;          /* remaining positions */
    idx_t ones_needed = k;    /* ones still to place */

    for (idx_t pos = 0; pos < 2*k; pos++) {
        seq_t count_one = 0;
        if (ones_needed > 0)
            count_one = binom(rem-1, ones_needed-1);

        if (offset < count_one) {
            result = result * 10 + 1;
            ones_needed -= 1;
        } else {
            result = result * 10 + 5;
            offset -= count_one;
        }
        rem -= 1;
    }
    return result;
}
