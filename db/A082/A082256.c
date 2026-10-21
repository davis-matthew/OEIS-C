/*
 * A082256  (Concatenation of 9n-8, 9n-7, 9n-6, 9n-5, 9n-4, 9n-3, 9n-2, 9n-1
 *          and 9n) divided by 9.
 * https://oeis.org/A082256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(9n-8, 9n-7, 9n-6, 9n-5, 9n-4, 9n-3, 9n-2, 9n-1,
 *            9n) / 9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to the integral type used for the sequence values */
    seq_t ns = (seq_t)n;
    seq_t result = 0;
    seq_t nine_n = ns * 9;           /* 9 * n                        */

    for (int i = 0; i < 9; ++i) {
        /* current number in the block 9n-8 to 9n */
        seq_t num = nine_n - 8 + i;

        /* find number of decimal digits of num */
        seq_t temp = num;
        int digits = 0;
        do {
            ++digits;
            temp /= 10;
        } while (temp > 0);

        /* compute 10^digits */
        seq_t pow10 = 1;
        for (int k = 0; k < digits; ++k) {
            pow10 *= 10;
        }

        /* append num to the concatenated value */
        result = result * pow10 + num;
    }

    /* divide by 9 to get the sequence term */
    return result / 9;
}
