/*
 * A138957  Concatenation of the reversed digits of numbers from 1 to n.
 * https://oeis.org/A138957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(reverse(1), reverse(2), …, reverse(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of the reversed digits of numbers from 1 to n. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t num = 1; num <= n; ++num)
    {
        /* count decimal digits of num */
        idx_t tmp = num;
        int digits = 0;
        while (tmp > 0) {
            ++digits;
            tmp /= 10;
        }

        /* compute reverse of num */
        idx_t rev_num = 0;
        tmp = num;
        while (tmp > 0) {
            rev_num = rev_num * 10 + (tmp % 10);
            tmp /= 10;
        }
        seq_t rev = (seq_t)rev_num;

        /* compute 10^digits */
        seq_t pow10 = 1;
        for (int i = 0; i < digits; ++i) {
            pow10 *= 10;
        }

        /* append rev to the current result */
        result = result * pow10 + rev;
    }
    return result;
}
