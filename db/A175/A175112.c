/*
 * A175112  First differences of A175111.
 * https://oeis.org/A175112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n+1)^5-(2*n-1)^5)/2+(-1)^n, n>0; a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t n_s = (seq_t)n;
    seq_t two = (seq_t)2;

    /* compute (2*n+1)^5 and (2*n-1)^5 */
    seq_t x = two * n_s + (seq_t)1;            /* 2*n+1 */
    seq_t y = two * n_s - (seq_t)1;            /* 2*n-1 */

    seq_t x2 = x * x;                          /* x^2 */
    seq_t x4 = x2 * x2;                        /* x^4 */
    seq_t x5 = x4 * x;                         /* x^5 */

    seq_t y2 = y * y;                          /* y^2 */
    seq_t y4 = y2 * y2;                        /* y^4 */
    seq_t y5 = y4 * y;                         /* y^5 */

    seq_t diff = x5 - y5;                      /* (2*n+1)^5 - (2*n-1)^5 */
    seq_t res = diff / (seq_t)2;                /* divide by 2 */

    /* add or subtract 1 depending on parity of n */
    if (n & (idx_t)1)
        res = res - (seq_t)1;                  /* (-1)^n = -1 when n odd */
    else
        res = res + (seq_t)1;                  /* (+1) when n even */

    return res;
}
