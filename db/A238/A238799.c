/*
 * A238799  a(0) = 1, a(n+1) = 2*a(n)^3 + 3*a(n).
 * https://oeis.org/A238799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n+1) = 2*a(n)^3 + 3*a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base value a(0) */
    seq_t val = 1;
    /* Iterate n times to reach a(n) */
    for (idx_t i = 0; i < n; ++i) {
        /* compute val = 2*val^3 + 3*val */
        /* multiplication order parenthesizes left‑to‑right, yielding 2*val^3 */
        val = (seq_t)2 * val * val * val + (seq_t)3 * val;
    }
    return val;
}
