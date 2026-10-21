/*
 * A220948  Expansion of (1-x)^2*(1-3*x)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A220948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5);
 *            a(0) = 1, a(1) = 4, a(2) = 15, a(3) = 55, a(4) = 200
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)55;
    if (n == 4) return (seq_t)200;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)15;
    seq_t a3 = (seq_t)55;
    seq_t a4 = (seq_t)200;

    idx_t i;
    for (i = 5; i <= n; ++i) {
        seq_t pos = ((seq_t)9) * a4 + ((seq_t)35) * a2 + ((seq_t)1) * a0;
        seq_t neg = ((seq_t)28) * a3 + ((seq_t)15) * a1;
        seq_t term = pos - neg;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = term;
    }
    return a4;
}
