/*
 * A192544  Bases b such that all integers m having the commuting property
 *          r(m)^2 = r(m^2), where r is cyclic replacement of digits d->(d+1)
 *          mod b, are of the form m = (b/2 - 1)*(b^k - 1)/(b - 1) + 1 for k
 *          >= 1.
 * https://oeis.org/A192544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(n + 1)
 * Range:     u8 n <= 62, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS entry specifies the bases as 8 + 4*k for k ≥ 0, i.e. a(n) = 4*(n+1) where the offset is 1. */
    return (seq_t)4 * ((seq_t)n + 1);
}
