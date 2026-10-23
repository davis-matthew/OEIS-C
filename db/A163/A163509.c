/*
 * A163509  Take n written in binary. Replace the leftmost run of 1's with
 *          just a single 1. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A163509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = remove all leading 1’s in the binary expansion of n and
 *            replace them with a single 1 bit (e.g., 11101 → 101)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* N≥1 by the OEIS offset; for safety return 0 if called with 0. */
    if (n == 0) return (seq_t)0;

    /* Position of the most significant set bit (0‑based). */
    idx_t msb = 63 - __builtin_clzll(n);        /* works for uint64_t */

    /* Scan downward for the first zero bit below the msb. */
    idx_t i = msb;
    while ( ((n >> i) & 1ULL) )
    {
        if (i == 0)          /* no zero found – all bits are 1 */
            return (seq_t)1;  /* result is a single bit “1” */
        --i;
    }
    /* i now holds the index of the first zero bit. */
    idx_t z = i;

    /* Bits below that zero stay unchanged. */
    seq_t low  = ((seq_t)n) & ( ((seq_t)1 << z) - 1 );

    /* Place a single ‘1’ bit immediately above that zero. */
    seq_t high = ((seq_t)1 << (z + 1));

    return low + high;
}
