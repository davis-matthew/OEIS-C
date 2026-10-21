/*
 * A202831  Expansion of e.g.f.: exp(4*x/(1-5*x)) / sqrt(1-25*x^2).
 * https://oeis.org/A202831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n-1)*a(n-1) + 5*(n-1)*(5*n-1)*a(n-2) -
 *            125*(n-1)*(n-2)^2*a(n-3) Initial values: a(0)=1, a(1)=4, a(2)=81
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)81;

    seq_t a0 = (seq_t)1;      /* a(n-3)  */
    seq_t a1 = (seq_t)4;      /* a(n-2)  */
    seq_t a2 = (seq_t)81;     /* a(n-1)  */
    seq_t ai = 0;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t factor = (seq_t)5 * i - 1;          /* (5*i - 1) */
        seq_t term1   = factor * a2;              /* (5*n-1)*a(n-1) */
        seq_t term2   = (seq_t)5 * (i - 1) * factor * a1; /* 5*(n-1)*(5*n-1)*a(n-2) */
        seq_t term3   = (seq_t)125 * (i - 1) * (i - 2) * (i - 2) * a0; /* 125*(n-1)*(n-2)^2*a(n-3) */
        ai = term1 + term2 - term3;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return ai;
}
