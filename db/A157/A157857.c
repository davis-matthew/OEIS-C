/*
 * A157857  a(n) = 3600*n^2 - n.
 * https://oeis.org/A157857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 - n
 * Range:     u8 none, u16 n <= 4, u32 n <= 1092, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute using a wider type to avoid overflow on intermediate steps */
    unsigned long long nn = (unsigned long long)n;
    seq_t val = (seq_t)(3600ULL * nn * nn - nn);
    return val;
}
