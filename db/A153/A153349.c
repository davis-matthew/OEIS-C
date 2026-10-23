/*
 * A153349  Period 6: repeat [1, 7, 4, 4, 7, 1].
 * https://oeis.org/A153349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence A153349: a(n) = [1, 7, 4, 4, 7, 1][n mod 6] */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {1, 7, 4, 4, 7, 1};
    return pat[n % 6];
}
