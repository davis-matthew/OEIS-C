/*
 * A215205  a(n) = (-1)^n * (A060819(n) + A060819(n+1)).
 * https://oeis.org/A215205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   a(4*n) = 1+5*n, a(1+4*n) = -2-6*n, a(2+4*n) = 4+6*n, a(3+4*n) =
 *            -4-5*n
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 4 */
    uint64_t r = n & 3ULL;
    /* floor division by 4, cast to seq_t for multiplication */
    seq_t q = (seq_t)(n / 4);

    switch (r) {
    case 0:                 /* n % 4 == 0  →  a(4*q) = 1 + 5*q */
        return 1 + 5 * q;
    case 1:                 /* n % 4 == 1  →  a(1+4*q) = -2 - 6*q */
        return -2 - 6 * q;
    case 2:                 /* n % 4 == 2  →  a(2+4*q) = 4 + 6*q */
        return 4 + 6 * q;
    case 3:                 /* n % 4 == 3  →  a(3+4*q) = -4 - 5*q */
        return -4 - 5 * q;
    }
    /* unreachable, but needs a return to satisfy the compiler */
    return 0;
}
