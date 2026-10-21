/*
 * A095340  Total number of nodes in all labeled graphs on n nodes.
 * https://oeis.org/A095340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n * 2^(n*(n-1)/2) from A095340
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the total number of nodes in all labeled graphs on n nodes. */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n == 0 the value is undefined, return 0. */
    if (n == 0) return (seq_t)0;

    /* Calculate exponent e = n*(n-1)/2 as an unsigned 64-bit integer. */
    uint64_t exp = ((uint64_t)n * (uint64_t)(n - 1)) / 2ULL;

    /* pow2 holds 2^e.  The shift operator works for seq_t. */
    seq_t pow2 = ((seq_t)1) << exp;

    /* Final result is n * 2^e. */
    return ((seq_t)n) * pow2;
}
