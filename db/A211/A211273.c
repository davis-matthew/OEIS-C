/*
 * A211273  Number of integer pairs (x,y) such that 0<x<=y<=n and x*y<=2n.
 * https://oeis.org/A211273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{x=1}^{n} max(0, min(n, ⌊2n/x⌋) − x + 1)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts pairs (x,y) with 1 ≤ x ≤ y ≤ n and x*y ≤ 2n. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t two = 2;
    seq_t sn = (seq_t)n;
    for (idx_t x = 1; x <= n; ++x) {
        seq_t bound = two * sn / (seq_t)x;          /* floor(2n/x) */
        if (bound < (seq_t)x) {
            /* no y ≥ x satisfies the inequality for this x */
            continue;
        }
        seq_t ymax = bound;
        if (ymax > sn) {
            ymax = sn;                              /* y cannot exceed n */
        }
        total += ymax - (seq_t)x + 1;               /* count of valid y */
    }
    return total;
}
