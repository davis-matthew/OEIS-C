/*
 * A204644  Number of (n+1) X 2 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(Fib(n+3)-1)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast doubling algorithm for Fibonacci */
    idx_t m = n + 3;          /* we need Fib(m) = Fib(n+3) */
    seq_t f0 = 0;              /* F(0)   */
    seq_t f1 = 1;              /* F(1)   */

    /* iterate over all bits of m from most to least significant */
    for (int i = (int)(sizeof(idx_t) * 8) - 1; i >= 0; --i) {
        idx_t bit = ((idx_t)1) << i;

        /* compute the doubling formulas
           c = F(2k)   = F(k) * (2*F(k+1) - F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t tmp = (f1 << 1) - f0;       /* 2*F(k+1) - F(k)   */
        seq_t c   = f0 * tmp;             /* F(2k)             */
        seq_t d   = f0 * f0 + f1 * f1;    /* F(2k+1)           */

        if (m & bit) {
            f0 = d;
            f1 = c + d;
        } else {
            f0 = c;
            f1 = d;
        }
    }

    /* f0 now holds Fib(m) = Fib(n+3) */
    return (f0 - 1) * 4;
}
