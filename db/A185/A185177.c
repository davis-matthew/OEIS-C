/*
 * A185177  Different values of the permanent in (0,1) quadratic matrices with
 *          exactly 3 ones in each row and each column, in the increasing
 *          order.
 * https://oeis.org/A185177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distinct permanents of 0‑1 matrices with exactly 3 ones per row/column.
   For n ≥ 12 all values occur consecutively: a(n) = n + 18.
   The first 11 values are: 6, 9, 12, 13, 17, 18, 20, 24, 25, 26, 27. */
seq_t a(idx_t n) {
    if (n <= 11) {
        static const seq_t vals[11] = {
            6, 9, 12, 13, 17, 18, 20, 24, 25, 26, 27
        };
        return vals[n - 1];
    }
    return (seq_t)n + 18;
}
