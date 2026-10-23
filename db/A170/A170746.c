/*
 * A170746  Expansion of g.f.: (1+x)/(1-26*x).
 * https://oeis.org/A170746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1, a(n)=27*26^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;          /* a(0) = 1 */

    /* Compute 26^(n-1) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0u; i < n - 1u; ++i)
        pow = pow * (seq_t)26;

    /* Multiply by 27 */
    return pow * (seq_t)27;
}
