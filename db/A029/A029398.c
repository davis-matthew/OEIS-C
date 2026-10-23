/*
 * A029398  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((85*n^3+84*n^2+69*n+54)/54) * -
 *            floor((3*n^3+4*n^2+2*n+2)/5) * - floor((3*n^3+4*n^2+4*n+4)/7) *
 *            - floor((6*n^3+2*n^2+3*n+4)/11)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t once */
    seq_t sn   = (seq_t)n;
    seq_t sq   = sn * sn;          /* n^2 */
    seq_t sc   = sq * sn;          /* n^3 */

    /* first large numerator and division by 54 */
    seq_t num1 = 85 * sc + 84 * sq + 69 * sn + (seq_t)54;
    seq_t a1   = num1 / (seq_t)54;

    /* second numerator and division by 5 */
    seq_t num2 = 3 * sc + 4 * sq + 2 * sn + (seq_t)2;
    seq_t a2   = num2 / (seq_t)5;

    /* third numerator and division by 7 */
    seq_t num3 = 3 * sc + 4 * sq + 4 * sn + (seq_t)4;
    seq_t a3   = num3 / (seq_t)7;

    /* fourth numerator and division by 11 */
    seq_t num4 = 6 * sc + 2 * sq + 3 * sn + (seq_t)4;
    seq_t a4   = num4 / (seq_t)11;

    /* compute the result */
    return a1 - a2 - a3 - a4;
}
