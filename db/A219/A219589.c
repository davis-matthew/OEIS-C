/*
 * A219589  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 2 array.
 * https://oeis.org/A219589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3. For n > 1: a(n) = (n^4 - 4*n^3 + 47*n^2 - 56*n + 24) /
 *            12 (which equals (1/12)*n^4 - (1/3)*n^3 + (47/12)*n^2 - (14/3)*n
 *            + 2)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    seq_t nn   = (seq_t)n;          /* n */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t n3   = n2 * nn;           /* n^3 */
    seq_t n4   = n3 * nn;           /* n^4 */

    /* Compute numerator = n4 - 4*n3 + 47*n2 - 56*nn + 24
       by arranging additions first to avoid unsigned underflow. */
    seq_t temp   = n4 + (seq_t)47 * n2 + (seq_t)24; /* n4 + 47*n2 + 24 */
    seq_t sub1   = (seq_t)4 * n3;                   /* 4*n3   */
    seq_t sub2   = (seq_t)56 * nn;                  /* 56*n   */
    seq_t numer  = temp - sub1 - sub2;              /* result */

    return numer / (seq_t)12;                      /* divide by 12 */
}
