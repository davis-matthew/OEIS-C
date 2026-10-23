/*
 * A017933  Expansion of 1/((1-3x)(1-6x)(1-9x)).
 * https://oeis.org/A017933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 8*6^n + 9*9^n)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n, 6^n and 9^n by repeated multiplication */
    seq_t pow3 = 1;
    seq_t pow6 = 1;
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;
        pow6 *= 6;
        pow9 *= 9;
    }

    /* arrange the arithmetic to avoid unsigned underflow:
       (3^n + 9*9^n) - 8*6^n */
    seq_t tmp = pow3 + 9 * pow9;
    tmp -= 8 * pow6;

    return tmp / 2;
}
