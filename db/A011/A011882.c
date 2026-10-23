/*
 * A011882  [ n(n-1)/29 ].
 * https://oeis.org/A011882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n*(n-1)/29⌋
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes term n of OEIS sequence A011882.
 *  The computation uses only integer arithmetic, avoiding underflow.
 *  For n = 0 or n = 1 the result is 0, matching the definition. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    seq_t nn = (seq_t)n;                /* convert to seq_t first */
    return (nn * (nn - 1)) / (seq_t)29; /* integer division truncates toward zero */
}
