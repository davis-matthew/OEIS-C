/*
 * A104474  a(n) = binomial(n+3,3)*binomial(n+7,3).
 * https://oeis.org/A104474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3,3)*binomial(n+7,3) =
 *            (1/36)*(n+1)*(n+2)*(n+3)*(n+5)*(n+6)*(n+7)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t f1 = x + 1;
    seq_t f2 = x + 2;
    seq_t f3 = x + 3;
    seq_t f4 = x + 5;
    seq_t f5 = x + 6;
    seq_t f6 = x + 7;

    /* cancel the denominator 36 = 2*2*3*3, dividing early to keep
       intermediate values small. */
    /* first factor of 2 */
    if (f1 % 2 == 0)     f1 /= 2;
    else if (f2 % 2 == 0) f2 /= 2;
    else if (f3 % 2 == 0) f3 /= 2;
    else if (f4 % 2 == 0) f4 /= 2;
    else if (f5 % 2 == 0) f5 /= 2;
    else                  f6 /= 2;

    /* second factor of 2 */
    if (f1 % 2 == 0)     f1 /= 2;
    else if (f2 % 2 == 0) f2 /= 2;
    else if (f3 % 2 == 0) f3 /= 2;
    else if (f4 % 2 == 0) f4 /= 2;
    else if (f5 % 2 == 0) f5 /= 2;
    else                  f6 /= 2;

    /* first factor of 3 */
    if (f1 % 3 == 0)     f1 /= 3;
    else if (f2 % 3 == 0) f2 /= 3;
    else if (f3 % 3 == 0) f3 /= 3;
    else if (f4 % 3 == 0) f4 /= 3;
    else if (f5 % 3 == 0) f5 /= 3;
    else                  f6 /= 3;

    /* second factor of 3 */
    if (f1 % 3 == 0)     f1 /= 3;
    else if (f2 % 3 == 0) f2 /= 3;
    else if (f3 % 3 == 0) f3 /= 3;
    else if (f4 % 3 == 0) f4 /= 3;
    else if (f5 % 3 == 0) f5 /= 3;
    else                  f6 /= 3;

    return f1 * f2 * f3 * f4 * f5 * f6;
}
