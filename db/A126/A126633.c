/*
 * A126633  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of digits
 *          1, 2, at least one of digits 3,4, at least one of digits 5,6 and
 *          at least one of digits 7,8,9.
 * https://oeis.org/A126633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 24*6^n - 60*5^n + 62*4^n - 33*3^n + 9*2^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = 1, pow5 = 1, pow4 = 1, pow3 = 1, pow2 = 1;
    idx_t i;
    for (i = 0; i < n; ++i) {
        pow6 *= (seq_t)6;
        pow5 *= (seq_t)5;
        pow4 *= (seq_t)4;
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
    }
    seq_t pos = (seq_t)24 * pow6 + (seq_t)62 * pow4 + (seq_t)9 * pow2;
    seq_t neg = (seq_t)60 * pow5 + (seq_t)33 * pow3 + (seq_t)1;
    return pos - neg;
}
