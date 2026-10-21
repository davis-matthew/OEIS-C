/*
 * A094259  Expansion of g.f.: (1-5*x)/((1-6*x)*(1-x)^2).
 * https://oeis.org/A094259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^(n+1) + 20*n + 19) / 25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n+1) = 6*a(n) - 4*n - 3   (for n ≥ 0) */
/* This computation works for any seq_t; for 64‑bit seq_t it is
   safe up to the point where the result still fits in 64 bits. */
seq_t a(idx_t n)
{
    seq_t val = 1;                      /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        /* Compute a(i+1) = 6*a(i) - 4*i - 3 */
        seq_t prod = 6 * val;
        seq_t sub  = (seq_t)(4 * i + 3);
        val = prod - sub;
    }
    return val;
}
