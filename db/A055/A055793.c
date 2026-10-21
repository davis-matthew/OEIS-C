/*
 * A055793  Numbers k such that k and floor[k/3] are both squares; i.e.,
 *          squares which remain squares when written in base 3 and last digit
 *          is removed.
 * https://oeis.org/A055793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 0 for n=1; otherwise let e = n-2, let (x_0,y_0) = (1,0)
 *            and (x_{k+1},y_{k+1}) = (2*x_k + 3*y_k, x_k + 2*y_k). Then a(n)
 *            = (x_e)^2. This is equivalent to the Pell equation x^2 - 3*y^2 =
 *            1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)0;   /* a(1) = 0 */
    idx_t e = n - (idx_t)2;               /* exponent for the Pell solution */
    seq_t x = (seq_t)1;                   /* initial x_0 */
    seq_t y = (seq_t)0;                   /* initial y_0 */
    for (idx_t i = (idx_t)0; i < e; ++i) {
        seq_t newx = (seq_t)(2) * x + (seq_t)(3) * y;   /* 2*x + 3*y */
        seq_t newy = x + (seq_t)(2) * y;                 /* x + 2*y */
        x = newx;
        y = newy;
    }
    return x * x;   /* result is the square of the Pell x */ 
}
