/*
 * A174785  Expansion of g.f. (1+2*x-x^2+x^3-x^4-x^5)/(1+x^3)^2.
 * https://oeis.org/A174785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(n) = 1 if n % 6 == 0 or 5 * a(n) = n + 1 if n % 6 == 1 *
 *            a(n) = -(n + 1) if n % 6 == 4 * a(n) = -1 if n % 6 == 2 or 3 *
 *            This pattern is derived from the closed form * a(n) =
 *            (n+4)*cos(pi*n/3)/3 + n*sin(pi*n/3)/sqrt(3) - (n+1)*(-1)^n/3. *
 *            The expression simplifies to the above periodic rule
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n % 6;
    seq_t res;
    switch (k) {
        case 0:
        case 5:
            res = 1;
            break;
        case 1:
            res = (seq_t)n + 1;
            break;
        case 4:
            res = -((seq_t)n + 1);
            break;
        default:           /* k == 2 or k == 3 */
            res = -1;
            break;
    }
    return res;
}
