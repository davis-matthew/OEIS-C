/*
 * A109111  a(n) = (1/sqrt(595))*((sqrt(595) + 26)*(125 + 5*sqrt(595))^n +
 *          (sqrt(595) - 26)*(125 - 5*sqrt(595))^n).
 * https://oeis.org/A109111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 250*a(n-1) - 750*a(n-2) with a(0)=2, a(1)=510 G.f.:
 *            2(1+5z)/(1-250z+750z^2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)510;
    seq_t a0 = (seq_t)2;   // a(0)
    seq_t a1 = (seq_t)510; // a(1)
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = (seq_t)250 * a1 - (seq_t)750 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
