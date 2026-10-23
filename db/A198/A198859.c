/*
 * A198859  a(n) = 2*9^n-1.
 * https://oeis.org/A198859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*9^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 9^n */
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow9 *= (seq_t)9;
    }
    /* a(n) = 2 * 9^n - 1 */
    return pow9 * (seq_t)2 - (seq_t)1;
}
