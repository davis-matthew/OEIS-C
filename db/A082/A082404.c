/*
 * A082404  Triangle in which n-th row gives trajectory of n under the map x
 *          -> x/2 if x is even, x -> x-1 if x is odd, stopping when we reach
 *          1.
 * https://oeis.org/A082404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n-th element of the flattened triangle defined by
 *            the map x -> x/2 if even, x -> x-1 if odd, stopping at 1
 * Range:     u8 n <= 2562, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   trajectory_len, trajectory_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/trajectory_len.h"
#include "../utils/trajectory_value.h"

/* Main accessor for the sequence. */
seq_t a(idx_t n) {
    /* Find the row that contains the n‑th term in the flat list. */
    idx_t cumulative = 0;
    idx_t row = 0;
    while (cumulative < n) {
        ++row;
        cumulative += trajectory_len(row);
    }

    /* Position inside that row. */
    idx_t pos_in_row = n - (cumulative - trajectory_len(row));   /* 1‑based */

    return trajectory_value(row, pos_in_row);
}
