/*
 * A070775  a(n) = Sum_{k=0..n} binomial(4*n,4*k).
 * https://oeis.org/A070775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n,4*k) Equivalent closed form for
 *            n>0: a(n) = (1/2)*(-4)^n + (1/4)*16^n Recurrence for n>=2:
 *            a(0)=1, a(1)=2 a(n) = 16*a(n-1) - 10*(-4)^(n-1) where (-4)^(n-1)
 *            is updated each step by multiplying with -4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Working values: a_prev holds a(k-1),
       X holds (-4)^(k-1) for the current step k. */
    seq_t a_prev = (seq_t)2;   /* a(1) */
    seq_t X = (seq_t)-4;       /* (-4)^1 */

    /* We multiply a_prev by 16 and subtract 10*X to get a(k). */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t tmp1 = (seq_t)16 * a_prev;   /* 16 * a(k-1) */
        seq_t tmp2 = (seq_t)10 * X;        /* 10 * (-4)^(k-1) */
        a_prev = tmp1 - tmp2;              /* a(k) = 16*a(k-1) - 10*(-4)^(k-1) */

        X = (seq_t)-4 * X; /* update X to (-4)^k */
    }

    return a_prev;
}
