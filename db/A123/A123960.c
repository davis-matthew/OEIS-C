/*
 * A123960  Expansion of 2*x^4 / (1 - 11*x + 45*x^2 - 80*x^3 + 49*x^4).
 * https://oeis.org/A123960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in
 *            2*x^4/(1-11*x+45*x^2-80*x^3+49*x^4). This satisfies the linear
 *            recurrence for n ≥ 8: a(n) = 11*a(n-1) – 45*a(n-2) + 80*a(n-3) –
 *            49*a(n-4), with the initial values a(1) = a(2) = a(3) = 0, a(4)
 *            = 2, a(5) = 22, a(6) = 152, a(7) = 842
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return 0;          /* a(1) = a(2) = a(3) = 0 */
    if (n == 4) return (seq_t)2;   /* a(4) = 2 */
    /* For n ≥ 5 compute s_{n-4} where s_k are coefficients of 1/(1-...).
       Recurrence: s_k = 11*s_{k-1} – 45*s_{k-2} + 80*s_{k-3} – 49*s_{k-4},
       with s_0 = 1 and s_{-1} = s_{-2} = s_{-3} = 0. */
    idx_t m = n - 4;                /* we need s_m */
    seq_t s0 = 1, s1 = 0, s2 = 0, s3 = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t new_s = 11*s0 - 45*s1 + 80*s2 - 49*s3;
        s3 = s2;
        s2 = s1;
        s1 = s0;
        s0 = new_s;
    }
    return 2*s0;                    /* a(n) = 2*s_{n-4} for n ≥ 4 */
}
