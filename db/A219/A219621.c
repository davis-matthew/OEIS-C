/*
 * A219621  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..2 n X 2 array.
 * https://oeis.org/A219621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 23*n^2 + 12*n)/12 (valid for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)              /* offset starts at 1; 0 is unused */
        return 0;

    seq_t s   = (seq_t)n;        /* cast index to seq_t early          */
    seq_t n2  = s * s;           /* n^2                                 */
    seq_t n4  = n2 * n2;         /* n^4                                 */
    seq_t num = n4 + (23 * n2) + (12 * s); /* numerator */
    return num / 12;             /* exact division for all integer n    */
}
