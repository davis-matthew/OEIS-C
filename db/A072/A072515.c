/*
 * A072515  Let u(1) = u(2) = v(1) = v(2) = 1, u(n+2) = u(n)+v(n+1), v(n+2) =
 *          abs(u(n)-v(n+1)), then a(n) = u(n).
 * https://oeis.org/A072515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 114, u64 n <= 229,
 *            u128 n <= 460, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   u(1) = u(2) = 1,
   v(1) = v(2) = 1,
   u(n+2) = u(n) + v(n+1),
   v(n+2) = |u(n) - v(n+1)|;
   a(n) = u(n)
   Offset: 1 */

seq_t a(idx_t n)
{
    if (n == 0)          /* defined only for n>=1 */
        return 0;

    if (n <= 2)          /* u(1) = u(2) = 1 */
        return 1;

    /* initial values: u(1)=1, u(2)=1, v(2)=1 */
    seq_t u_prevprev = 1;  /* u(n-2)  */
    seq_t u_prev     = 1;  /* u(n-1)  */
    seq_t v_prev     = 1;  /* v(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t u_new = u_prevprev + v_prev;                /* u(i)   */
        seq_t v_new;
        if (u_prevprev >= v_prev)
            v_new = u_prevprev - v_prev;
        else
            v_new = v_prev - u_prevprev;                 /* |u(i-2)-v(i-1)| */

        /* shift for next iteration */
        u_prevprev = u_prev;
        u_prev     = u_new;
        v_prev     = v_new;
    }

    return u_prev;   /* u(n) */
}
