/*
 * A076899  Let u(1)=u(2)=u(3)=1, u(n)=sign(u(n-1)-u(n-2))/(u(n-3)+1); then
 *          a(n) is the denominator of u(n).
 * https://oeis.org/A076899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = den(u(n)) where u(1)=u(2)=u(3)=1,
 *            u(n)=sign(u(n‑1)–u(n‑2))/(u(n‑3)+1). The sequence satisfies:
 *            a(6k) = F(k+2); a(6k+1) = 1; a(6k+2) = a(6k+3) = F(k+1); a(6k+4)
 *            = 1; a(6k+5) = F(k+3); with F(0)=0, F(1)=1
 * Range:     u8 n <= 70, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t fib(idx_t m)
{
    /* Return the m‑th Fibonacci number (m ≥ 0). */
    if (m <= 1) return m;
    seq_t a = 0, b = 1;
    for (idx_t i = 1; i < m; ++i) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;  /* safety for offset start at 1 */

    idx_t r = n % 6;
    switch (r) {
        case 1: /* a(6k+1) = 1 */
        case 4: /* a(6k+4) = 1 */
            return 1;
        case 2: /* a(6k+2) = F(k+1) */
        case 3: /* a(6k+3) = F(k+1) */
        {
            idx_t k = (n - 2) / 6;
            return fib(k + 1);
        }
        case 5: /* a(6k+5) = F(k+3) */
        {
            idx_t k = (n - 5) / 6;
            return fib(k + 3);
        }
        case 0: /* a(6k) = F(k+2) */
        {
            idx_t k = n / 6;
            return fib(k + 2);
        }
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
