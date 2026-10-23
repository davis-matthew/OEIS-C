/*
 * A125880  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of the
 *          digits 1,2 and at least one of the digits 3,4,5,6,7,8,9.
 * https://oeis.org/A125880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 14*8^n-49*7^n+91*6^n-105*5^n+77*4^n-35*3^n+9*2^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute powers by repeated multiplication */
    seq_t p2 = (seq_t)1, p3 = (seq_t)1, p4 = (seq_t)1, p5 = (seq_t)1;
    seq_t p6 = (seq_t)1, p7 = (seq_t)1, p8 = (seq_t)1;
    seq_t base2 = (seq_t)2, base3 = (seq_t)3, base4 = (seq_t)4, base5 = (seq_t)5;
    seq_t base6 = (seq_t)6, base7 = (seq_t)7, base8 = (seq_t)8;

    idx_t i;
    for (i = n; i > 0; --i) {
        p2 *= base2;
        p3 *= base3;
        p4 *= base4;
        p5 *= base5;
        p6 *= base6;
        p7 *= base7;
        p8 *= base8;
    }

    seq_t result = (seq_t)14 * p8;
    result -= (seq_t)49 * p7;
    result += (seq_t)91 * p6;
    result -= (seq_t)105 * p5;
    result += (seq_t)77 * p4;
    result -= (seq_t)35 * p3;
    result += (seq_t)9 * p2;
    result -= (seq_t)1;
    return result;
}
