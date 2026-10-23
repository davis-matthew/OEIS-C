/*
 * A101381  a(n) = n^2*(n+1)^2*(4*n^2 - 5*n + 4)/12.
 * https://oeis.org/A101381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)^2*(4*n^2 - 5*n + 4)/12
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = x + 1;

    seq_t f[3];
    f[0] = x * x;                    /* n^2 */
    f[1] = y * y;                    /* (n+1)^2 */
    f[2] = (4 * x * x - 5 * x + 4); /* 4*n^2 - 5*n + 4 */

    /* 12 = 4 * 3.  One of f[0] or f[1] is divisible by 4;
       one of f[0], f[1], f[2] is divisible by 3.
       Divide out these factors to avoid overflow while keeping exactness. */
    static const unsigned divs[2] = {4, 3};
    for (int k = 0; k < 2; ++k) {
        unsigned d = divs[k];
        for (int i = 0; i < 3; ++i) {
            if ((f[i] % d) == 0) {
                f[i] /= d;
                break;
            }
        }
    }

    seq_t res = f[0];
    res *= f[1];
    res *= f[2];
    return res;
}
