/*
 * A081822  Outdated value of A070064.
 * https://oeis.org/A081822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = decimal digit of the constant 8.314472, with zeros
 *            appended after the 7th digit
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 7, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) enumerates the digits of the outdated value of R = 8.314472.  For n > 7 all digits are 0. */
seq_t a(idx_t n)
{
    /* digits of 8.314472: 8, 3, 1, 4, 4, 7, 2 */
    static const seq_t digits[] = {8, 3, 1, 4, 4, 7, 2};
    if (n <= 7) {
        return digits[n - 1];
    } else {
        return 0;
    }
}
