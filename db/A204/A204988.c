/*
 * A204988  The index j < k such that n divides 2^k - 2^j, where k is the
 *          least index (A204987) for which such j exists.
 * https://oeis.org/A204988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = max(1, v₂(n)), where v₂(n) is the exponent of 2 dividing
 *            n
 * Range:     u8 n <= 6556, u16 n <= 6556, u32 n <= 6556, u64 n <= 6556,
 *            u128 n <= 6556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t e   = 0;
    while ((tmp & 1ULL) == 0)
    {
        ++e;
        tmp >>= 1;
    }
    return e ? (seq_t)e : (seq_t)1;
}
