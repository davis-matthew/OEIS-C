/*
 * A155615  a(n) = 10^n - 3^n + 1.
 * https://oeis.org/A155615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 3^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    seq_t pow3  = 1;
    idx_t i;

    for (i = 0; i < n; ++i) {
        pow10 *= (seq_t)10;
        pow3  *= (seq_t)3;
    }

    return pow10 - pow3 + (seq_t)1;
}
