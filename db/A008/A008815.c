/*
 * A008815  Expansion of (1+x^8)/((1-x)^2*(1-x^8)).
 * https://oeis.org/A008815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-6)*floor((n+1)/8) - 8*floor((n+1)/8)^2 + n + 1
 * Range:     u8 n <= 44, u16 n <= 723, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* floor((n+1)/8) */
    idx_t k_idx = (n + 1) / 8;   /* integer division truncates */
    seq_t k = (seq_t)k_idx;

    seq_t res = sn + 1;   /* n + 1 */

    if (k_idx > 0) {
        /* (2*n-6) is non‑negative for all n with k_idx > 0 */
        seq_t t1 = sn * 2 - 6;
        seq_t term1 = t1 * k;
        res += term1;
        seq_t term2 = (seq_t)8 * k * k;
        res -= term2;
    }

    return res;
}
