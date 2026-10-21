/*
 * A053487  E.g.f.: exp(4x)/(1-x).
 * https://oeis.org/A053487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = n * a(n-1) + 4ⁿ (recurrence derived
 *            from a(n) = n! * Σ_{k=0}^{n} 4ᵏ / k!) Computes the permanent of
 *            the n×n matrix with 5's on the diagonal and 1's elsewhere (OEIS
 *            A053487)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)1;   /* a(0) */
    seq_t pow4  = (seq_t)1;   /* 4⁰ */

    for (idx_t i = 1; i <= n; ++i) {
        pow4 = pow4 * (seq_t)4;          /* pow4 = 4ⁱ */
        a_val = (seq_t)i * a_val + pow4; /* aₙ = i * a_{i-1} + 4ⁱ */
    }
    return a_val;
}
