/*
 * A171064  G.f.: -x*(x-1)*(1+x)/(1-x-7*x^2-x^3+x^4).
 * https://oeis.org/A171064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+7*a(n-2)+a(n-3)-a(n-4) with a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=7
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Directly return the first four terms. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;

    /* Variables hold the last four computed values:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    seq_t a3 = (seq_t)7;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + (seq_t)7 * a2 + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* a(n) */
}
