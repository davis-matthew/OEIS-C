/*
 * A200887  Number of 0..n arrays x(0..3) of 4 elements without any interior
 *          element greater than both neighbors.
 * https://oeis.org/A200887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+n)^2 * (3 + 5*n + n^2) / 3
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent polynomial: (1/3)*n^4 + (7/3)*n^3 + (14/3)*n^2 + (11/3)*n + 1) */

seq_t a(idx_t n)
{
    /* cast n to seq_t once */
    seq_t N   = (seq_t)n;
    seq_t n1  = N + 1;            /* (n + 1) */
    seq_t n1sq = n1 * n1;         /* (n + 1)^2 */
    seq_t inner = N * N + 5 * N + 3; /* n^2 + 5n + 3 */
    return n1sq * inner / 3;      /* integer division – exact for integer n */
}
