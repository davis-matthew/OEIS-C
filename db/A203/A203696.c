/*
 * A203696  v(n+1)/v(n), where v=A203695.
 * https://oeis.org/A203696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (T_i^2 + T_{n+1}^2), T_k = k(k+1)/2;
 *            this equals v(n+1)/v(n) for v=A203695
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n + 1) * ((seq_t)n + 2) / 2;
    seq_t t2 = t * t;
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ti = (seq_t)i * ((seq_t)i + 1) / 2;
        prod *= (ti * ti + t2);
    }
    return prod;
}
