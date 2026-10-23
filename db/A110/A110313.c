/*
 * A110313  Expansion of e.g.f. exp(x)/(1-x-x^2).
 * https://oeis.org/A110313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = (n+4)*a(n+2) + (n+2)^2*a(n+1) - (n+2)*(n+1)*a(n);
   a(0)=1, a(1)=2, a(2)=7 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 7;

    seq_t a0 = 1, a1 = 2, a2 = 7;  /* a(0), a(1), a(2) */
    idx_t k;
    for (k = 0; k <= n - 3; ++k) {
        seq_t term1 = (seq_t)(k + 4) * a2;
        seq_t term2 = (seq_t)((k + 2) * (k + 2)) * a1;
        seq_t term3 = (seq_t)((k + 2) * (k + 1)) * a0;
        seq_t an = term1 + term2 - term3;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
