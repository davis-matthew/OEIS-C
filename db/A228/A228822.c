/*
 * A228822  Largest digit of n minus the number of digits of n.
 * https://oeis.org/A228822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = largest digit of n minus number of digits of n
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int maxdigit = 0;
    int digits   = 0;
    idx_t temp   = n;

    if (temp == 0) {
        /* n = 0: largest digit 0, number of digits 1 */
        maxdigit = 0;
        digits   = 1;
    } else {
        while (temp != 0) {
            int d = (int)(temp % 10);
            if (d > maxdigit)
                maxdigit = d;
            ++digits;
            temp /= 10;
        }
    }
    return (seq_t)maxdigit - (seq_t)digits;
}
