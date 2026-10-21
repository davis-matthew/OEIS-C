/*
 * A078338  Let u(1)=u(2)=u(3)=1 and u(n)=(-1)^n*sign(u(n-1)-u(n-2))*u(n-3),
 *          then a(n)=sum(k=1,n,u(k)).
 * https://oeis.org/A078338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} u(k) where u(1)=u(2)=u(3)=1 and u(n)=(-1)^n *
 *            sign(u(n-1)-u(n-2)) * u(n-3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset: a(1) is the first term */
    if (n == 0)
        return 0;           /* safety, not used for normal offset */

    if (n <= 3)              /* u(1)=u(2)=u(3)=1 ⇒ a(n)=n for n≤3 */
        return (seq_t)n;

    /* a(3) = 3 */
    int acc = 3;

    /* maintain a sliding window of three consecutive u‑values:
       u_prev3 = u(k-3), u_prev2 = u(k-2), u_prev1 = u(k-1)   */
    int u_prev3 = 1;      /* u(1) */
    int u_prev2 = 1;      /* u(2) */
    int u_prev1 = 1;      /* u(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* sign(u(i-1) - u(i-2)) */
        int diff  = u_prev1 - u_prev2;
        int sign  = (diff > 0) - (diff < 0);   /* -1, 0, or +1 */

        /* (-1)^i  →  i even gives +1, i odd gives -1 */
        int parity = (i & 1) ? -1 : 1;

        int new_u = parity * sign * u_prev3;  /* u(i) */

        acc += new_u;

        /* shift window for next iteration */
        u_prev3 = u_prev2;
        u_prev2 = u_prev1;
        u_prev1 = new_u;
    }

    return (seq_t)acc;
}
