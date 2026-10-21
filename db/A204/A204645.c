/*
 * A204645  Number of (n+1) X 3 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (576 + 704*n + 232*n^2 + 16*n^3 + 2*n^4)/96 for n even, *
 *            a(n) = (582 + 704*n + 232*n^2 + 16*n^3 + 2*n^4)/96 for n odd
 * Range:     u8 none, u16 n <= 9, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* out of range – sequence is defined for n ≥ 1 */
        return 0;

    /* Promote n to seq_t once to avoid repeated casts. */
    seq_t N = (seq_t)n;

    /* Compute the polynomial terms. Multiplications are done left–to–right,
       using only operations allowed on seq_t. */
    seq_t t1 = 704 * N;                     /* 704*n */
    seq_t t2 = 232 * N * N;                 /* 232*n^2 */
    seq_t t3 = 16 * N * N * N;              /* 16*n^3 */
    seq_t t4 = 2 * N * N * N * N;           /* 2*n^4 */

    seq_t numerator;

    /* Choose the constant term depending on the parity of n. */
    if ((n & 1) == 0)   /* n even */
        numerator = 576 + t1 + t2 + t3 + t4;
    else                /* n odd  */
        numerator = 582 + t1 + t2 + t3 + t4;

    /* The division is exact – the result is an integer. */
    return numerator / 96;
}
