/*
 * A051872  20-gonal (or icosagonal) numbers: a(n) = n*(9*n-8).
 * https://oeis.org/A051872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n-8)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type to avoid mixing signed and unsigned
       arithmetic and ensure correctness for arbitrary-precision builds. */
    seq_t sn = (seq_t)n;
    /* Compute the 20-gonal number using the closed-form expression. */
    return sn * (sn * (seq_t)9 - (seq_t)8);
}
