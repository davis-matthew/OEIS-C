/*
 * A195857  a(n) = T(9,n), array T given by A047858.
 * https://oeis.org/A195857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(n+18) - 8 (a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute 2^(n-1) in a loop to avoid overflow on built‑in types */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow2 *= (seq_t)2;

    seq_t mult = (seq_t)n + (seq_t)18;
    seq_t res  = pow2 * mult - (seq_t)8;
    return res;
}
