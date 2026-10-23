/*
 * A017494  a(n) = (11*n + 8)^10.
 * https://oeis.org/A017494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 647,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (11*n + 8) in seq_t.  The cast to seq_t is
       necessary to avoid overflow when multiplying the idx_t value. */
    seq_t base = ((seq_t)n) * (seq_t)11 + (seq_t)8;

    /* Raise the base to the 10th power by repeated multiplication.
       An explicit loop avoids reliance on a missing pow_seq helper. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 10; ++i)
        result *= base;

    return result;
}
