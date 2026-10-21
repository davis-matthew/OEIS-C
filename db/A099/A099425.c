/*
 * A099425  Expansion of (1+x^2)/(1-2*x-x^2).
 * https://oeis.org/A099425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((1+sqrt(2))^n) Recurrence: a(0)=1, a(1)=2, a(2)=6,
 *            a(n)=2*a(n-1)+a(n-2) for n≥3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    /* iterative computation */
    seq_t a_nm2 = (seq_t)2;  /* a(n-2) for the first iteration (a1) */
    seq_t a_nm1 = (seq_t)6;  /* a(n-1) for the first iteration (a2) */
    seq_t a_n   = a_nm1;     /* placeholder for current value */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 2 * a_nm1 + a_nm2;  /* a(i) = 2*a(i-1)+a(i-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
