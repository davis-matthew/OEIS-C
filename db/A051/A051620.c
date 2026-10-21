/*
 * A051620  a(n) = (4*n+8)(!^4)/8(!^4), related to A034177(n+1) ((4*n+4)(!^4)
 *          quartic, or 4-factorials).
 * https://oeis.org/A051620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; * a(n) = 4*(n+2) * a(n-1) for n ≥ 1 * This recurrence
 *            follows from the e.g.f. 1/(1-4x)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = (seq_t)(4ULL * (i + 2));
        val = val * mult;         /* val now holds a(i) */
    }
    return val;
}
