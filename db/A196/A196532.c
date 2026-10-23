/*
 * A196532  a(n) = (n+1)!*(H(n)+H(n+1)), where H(n) = Sum_{k=1..n} 1/k is the
 *          n-th harmonic number.
 * https://oeis.org/A196532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)!*(H(n)+H(n+1)) Recurrence (OEIS A196532): a(n+3) =
 *            (3*n+8)*a(n+2) - (3*n+7)*(n+2)*a(n+1) + (n+1)*(n+2)^2*a(n)
 *            Initial values: a(0)=1, a(1)=5, a(2)=20
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 20;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 5;   /* a(n-2) */
    seq_t a2 = 20;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        idx_t k = i - 3;                     /* original n in the recurrence */

        seq_t coeff1 = (seq_t)(3 * k + 8);
        seq_t coeff2 = (seq_t)(3 * k + 7);
        idx_t mult2 = k + 2;                 /* i - 1 */
        seq_t coeff3 = (seq_t)(k + 1);
        idx_t mult3 = k + 2;                 /* i - 1 */

        seq_t term1 = coeff1 * a2;
        seq_t term2 = coeff2 * (seq_t)mult2 * a1;
        seq_t term3 = coeff3 * (seq_t)mult3 * (seq_t)mult3 * a0;

        seq_t cur = term1 + term3;
        cur -= term2;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
