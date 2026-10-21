/*
 * A091711  Exponent of 2 in (n^2)!.
 * https://oeis.org/A091711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k≥1} ⌊(n²)/2ᵏ⌋ (Legendre's formula for v₂((n²)!) )
 * Range:     u8 n <= 11, u16 n <= 158, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n² as seq_t to avoid overflow of the index type */
    seq_t m = (seq_t)n * (seq_t)n;
    seq_t exp = 0;
    /* accumulate floor divisions by powers of two */
    for (seq_t d = 2; d <= m; d <<= 1)
        exp += m / d;
    return exp;
}
