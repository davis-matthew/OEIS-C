/*
 * A020531  a(n) = 5th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3*4^n + 16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = 1;
    seq_t pow16 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4  *= 4;
        pow16 *= 16;
    }
    return 1 + (seq_t)3 * pow4 + pow16;
}
