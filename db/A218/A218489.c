/*
 * A218489  The sequence of coefficients of cubic polynomials p(x+n), where
 *          p(x) = x^3 - 3*x + 1.
 * https://oeis.org/A218489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n % 4 == 0 * a(n) = 3*(n/4) if n % 4 == 1 * a(n) =
 *            3*(n/4)^2 - 3 if n % 4 == 2 * a(n) = (n/4)^3 - 3*(n/4) + 1 if n
 *            % 4 == 3 * Low‑order terms: 1,0,-3,1,1,3,0,-1,
 * Range:     i8 n <= 26, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 2;         /* n divided by 4 */
    idx_t r = n & 3;          /* n modulo 4 */
    seq_t K = (seq_t)k;

    switch (r) {
    case 0: return 1;
    case 1: return 3 * K;
    case 2: return 3 * K * K - 3;
    case 3: return K * K * K - 3 * K + 1;
    default: return 0;        /* unreachable */
    }
}
