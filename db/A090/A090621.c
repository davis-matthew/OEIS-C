/*
 * A090621  Exponent of highest power of 16 dividing n!.
 * https://oeis.org/A090621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n - popcount(n)) / 4) (exponent of 16 in n!)
 * Range:     u8 n <= 466, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use built-in popcount on the 64‑bit index.  The result fits in seq_t. */
    unsigned int bits = __builtin_popcountll(n);
    seq_t v2 = (seq_t)n - (seq_t)bits;   /* exponent of 2 in n!  */
    return v2 / 4;                       /* exponent of 16 in n! */
}
