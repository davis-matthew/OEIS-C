/*
 * A069960  Define C(n) by the recursion C(0) = 3*i where i^2 = -1, C(n+1) =
 *          1/(1 + C(n)), then a(n) = 3*(-1)^n/Im(C(n)) where Im(z) denotes
 *          the imaginary part of the complex number z.
 * https://oeis.org/A069960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*F(n)^2 + F(n+1)^2 Recurrence: a(0)=1, a(1)=10, a(2)=13;
 *            a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)10;
    if (n == 2u) return (seq_t)13;

    seq_t a0 = (seq_t)1,   /* a(n-3)   */
          a1 = (seq_t)10,  /* a(n-2)   */
          a2 = (seq_t)13;  /* a(n-1)   */
    seq_t an;

    for (idx_t i = 3u; i <= n; ++i) {
        /* an = 2*a2 + 2*a1 - a0 */
        an = a2; an += a2;       /* 2*a2   */
        seq_t tmp = a1; tmp += a1; /* 2*a1   */
        an += tmp;
        an -= a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
