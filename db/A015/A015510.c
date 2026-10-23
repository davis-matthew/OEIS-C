/*
 * A015510  Inverse of 1501st cyclotomic polynomial.
 * https://oeis.org/A015510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1/Φ_1501(x), where Φ_1501(x) is the 1501st cyclotomic
 *            polynomial. The sequence is periodic of period 1501. Let r = n
 *            mod 1501. Then a(n) = 1 if 0 <= r <= 18 a(n) = 0 if 19 <= r <=
 *            78 or 98 <= r <= 1500 a(n) = -1 if 79 <= r <= 97
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 1501;
    if (r <= 18)
        return 1;
    if (r <= 78)
        return 0;
    if (r <= 97)
        return -1;
    return 0;
}
