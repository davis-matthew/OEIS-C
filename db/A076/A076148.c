/*
 * A076148  Let b(1)=x, b(2)=y, k*b(k)=(2k-1)*b(k-1) + 3(k+1)*b(k-2); then
 *          b(n)=a(n)*x+c(n)/3*y.
 * https://oeis.org/A076148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥1: a(n+1)=3*a(n)+(-1)^n*(n+2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety */
    if (n == 0) return 0;

    seq_t cur = (seq_t)1;          /* a(1) */

    for (idx_t i = 1; i < n; ++i)  /* i = n-1 ⇒ compute a(i+1) ⇒ a(n) */
    {
        seq_t term = (seq_t)(i + 2);       /* (n+2) with n = i */
        cur = cur * (seq_t)3;
        if (i & 1)                         /* odd i → (-1)^i = -1 */
            cur = cur - term;
        else                               /* even i → (-1)^i = +1 */
            cur = cur + term;
    }
    return cur;
}
