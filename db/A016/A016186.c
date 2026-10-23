/*
 * A016186  Expansion of 1/((1-8*x)*(1-10*x)).
 * https://oeis.org/A016186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*10^n - 4*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    seq_t pow8  = 1;
    /* compute 10^n and 8^n */
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= 10;
        pow8  *= 8;
    }
    return 5 * pow10 - 4 * pow8;
}
