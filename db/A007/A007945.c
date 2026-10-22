/*
 * A007945  Expansion of g.f. (2-x-2*x^2)/((1-x)*(1-x+x^2)).
 * https://oeis.org/A007945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*sin(n*Pi/3)/sqrt(3) + 3*cos(n*Pi/3) - 1 (period 6)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t period[6] = {2, 3, 0, -4, -5, -2};
    return period[n % 6];
}
