/*
 * A166796  Number of nX2 1..3 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 8*n^3 + 11*n^2 - 32*n + 12)/12
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 (outside the OEIS offset) return 0. */
    if (n == 0) {
        return 0;
    }

    /* Cast the index to seq_t to guarantee the same type for all
       arithmetic operations, as required. */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;            /* n^2   */
    seq_t n3  = n2 * sn;            /* n^3   */
    seq_t n4  = n3 * sn;            /* n^4   */

    /* Numerator: n^4 + 8*n^3 + 11*n^2 + 12 - 32*n */
    seq_t numerator = n4
                     + (seq_t)8 * n3
                     + (seq_t)11 * n2
                     + (seq_t)12;

    /* Subtract 32*n now that the sum is guaranteed to be
       at least 32*n for all n >= 1. */
    numerator -= (seq_t)32 * sn;

    /* Denominator is 12. */
    return numerator / (seq_t)12;
}
