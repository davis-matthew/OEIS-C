/*
 * A174256  Number of 3 X 3 reduced magic squares with distinct values and
 *          maximum value 2n; also, with magic sum 3n.
 * https://oeis.org/A174256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(6*n - 13 - 8*cos(2*n*PI/3) - 3*cos(n*PI))/3
 *            Implemented as a piecewise linear function using integer
 *            arithmetic
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    unsigned mod2 = n & 1U;   /* 0 if n even, 1 if odd */
    unsigned mod3 = n % 3U;   /* 0, 1, or 2 */
    seq_t result;
    if (mod3 == 0U)           /* cos(2*n*PI/3) = +1 */
    {
        if (mod2 == 0U)       /* cos(n*PI) = +1  */
            result = 4*N - 16;
        else                  /* cos(n*PI) = -1  */
            result = 4*N - 12;
    }
    else                      /* cos(2*n*PI/3) = -1/2 */
    {
        if (mod2 == 0U)       /* cos(n*PI) = +1  */
            result = 4*N - 8;
        else                  /* cos(n*PI) = -1  */
            result = 4*N - 4;
    }
    return result;
}
