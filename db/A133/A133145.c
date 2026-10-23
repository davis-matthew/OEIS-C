/*
 * A133145  Period 4: repeat [1, 2, 4, 8].
 * https://oeis.org/A133145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n mod 4)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Table of period‑4 values: 1, 2, 4, 8 */
    static const seq_t tbl[4] = {1, 2, 4, 8};
    return tbl[n & 3];
}
