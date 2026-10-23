/*
 * A123952  Sum of first row of the 5 X 5 matrix M^n, where M = {{5,-1,0,0,0},
 *          {-1,5,-1,0,0}, {0,-1,5,-1,0}, {0,0,-1,5,-1}, {0,0,0,-1,5}}.
 * https://oeis.org/A123952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*5^n +(5-sqrt(3))^n*(2+sqrt(3))
 *            -(-2+sqrt(3))*(5+sqrt(3))^n)/6 Equivalent integer recurrence:
 *            with x_n + y_n*sqrt(3) = (5+sqrt(3))^n a(n) = (5^n + 2*x_n -
 *            3*y_n)/3 Recurrence for x_n,y_n and 5^n: x_{k+1} = 5*x_k + 3*y_k
 *            y_{k+1} = x_k + 5*y_k 5^{k+1} = 5*5^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return 1;
    /* Initialize variables for 5^k, x_k, y_k at k=0 */
    seq_t pow5   = 1;   /* 5^0 */
    seq_t x      = 1;   /* x_0 = 1   */
    seq_t y      = 0;   /* y_0 = 0   */

    /* Iterate k = 0 .. n-1, updating pow5, x, y to k+1 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t new_pow5 = pow5 * 5;
        seq_t new_x    = x * 5 + y * 3;
        seq_t new_y    = x + y * 5;

        pow5 = new_pow5;
        x    = new_x;
        y    = new_y;
    }

    /* Compute numerator = 5^n + 2*x - 3*y   (always divisible by 3) */
    seq_t numerator = pow5 + (x * 2) - (y * 3);

    /* Final division by 3 */
    return numerator / 3;
}
