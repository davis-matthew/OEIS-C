/*
 * A115056  a(n) = n*(n^2-1)*(3*n+2).
 * https://oeis.org/A115056
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2-1)*(3*n+2)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 194, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Returns 0 for the initial indices (-1,0,1) as per the OEIS offset. */
    if (n <= 1)
        return 0;

    /* Compute the value using seq_t only. */
    seq_t tn   = (seq_t)n;          /* cast index to seq_t for the first operation */
    seq_t t2   = tn * tn;           /* n^2  (no overflow for all values that fit in seq_t) */
    seq_t part = tn * (t2 - 1);     /* n*(n^2-1) */
    seq_t mul  = 3 * tn + 2;        /* 3*n+2 */
    return part * mul;              /* final result */
}
