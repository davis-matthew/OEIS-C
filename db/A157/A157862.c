/*
 * A157862  a(n) = 1728000*n + 240.
 * https://oeis.org/A157862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728000*n + 240
 * Range:     u8 none, u16 none, u32 n <= 2485, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the linear function.  The intermediate multiplication is
       performed in 64 bits to avoid overflow of seq_t for small types. */
    return (seq_t)((uint64_t)n * 1728000ULL + 240ULL);
}
