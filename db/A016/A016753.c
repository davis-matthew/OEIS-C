/*
 * A016753  Expansion of 1/((1-3*x)*(1-4*x)*(1-5*x)).
 * https://oeis.org/A016753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n+2)/2 - 4^(n+2) + 3^(n+2)/2, n ≥ 0. Equivalent
 *            recurrence: a(0)=1, a(1)=12, a(n) = 9*a(n-1) - 20*a(n-2) + 3^n
 *            (n ≥ 2) Uses only integer arithmetic, works for both small
 *            (uint64_t) and arbitrary‑precision seq_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)12;  /* a(n-1) */
    seq_t power3 = (seq_t)9;  /* 3^2 for n=2 */

    seq_t a_n = 0;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)(9) * a_nm1 - (seq_t)(20) * a_nm2 + power3;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
        power3 *= (seq_t)3;  /* next 3^i */
    }
    return a_n;
}
