/*
 * A016887  a(n) = (5*n+3)^3.
 * https://oeis.org/A016887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 324, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5*n+3)^3
 * Computes the n-th term of OEIS sequence A016887.
 */
seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;   /* Convert index to the same type as result */
    t = t * 5 + 3;        /* Compute (5*n + 3) */
    t = t * t * t;        /* Cube the result */
    return t;
}
