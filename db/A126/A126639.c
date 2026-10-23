/*
 * A126639  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digit 1,2,3, at least one of
 *          digits 4,5,6 and at least one of digits 7,8,9.
 * https://oeis.org/A126639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*5^n - 18*4^n + 15*3^n - 6*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute powers 2^n, 3^n, 4^n and 5^n by repeated multiplication.
       For n up to 10 000 this is fast enough and requires only O(n) multiplications. */
    seq_t pow2 = 1;
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
        pow4 *= 4;
        pow5 *= 5;
    }

    seq_t result = 9 * pow5;
    result -= 18 * pow4;
    result += 15 * pow3;
    result -= 6 * pow2;
    result += 1;

    return result;
}
