/*
 * A170749  Expansion of g.f.: (1+x)/(1-29*x).
 * https://oeis.org/A170749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 30 * 29^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* Compute 29^(n-1) */
    seq_t pow29 = (seq_t)1;
    idx_t i;
    for (i = 0; i < n - 1; ++i) {
        pow29 *= (seq_t)29;
    }
    return (seq_t)30 * pow29;
}
