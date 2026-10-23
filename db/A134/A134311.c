/*
 * A134311  Row sums of triangle A134310.
 * https://oeis.org/A134311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = (n+3)*2^(n-1) - n - 1 for n > 0
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^(n-1) iteratively.  This works for all seq_t types
       (built‑in integer types or arbitrary precision).  For unsigned
       integral types overflowing would wrap modulo 2^bits – but all
       values that fit in the target type are computed correctly. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow2 <<= 1;

    /* Assemble the formula using only integer arithmetic. */
    seq_t term = ((seq_t)(n + 3)) * pow2;
    term -= (seq_t)n;
    term -= (seq_t)1;

    return term;
}
