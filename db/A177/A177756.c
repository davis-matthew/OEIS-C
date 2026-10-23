/*
 * A177756  Number of ways to place 3 nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A177756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/12*(n-2)^2*n^2*(2*n^2-4*n+5+3*(-1)^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n=1,2 because the formula is 0 there. */
    if (n < 3)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t nm2 = (seq_t)(n - 2);          /* (n-2) with n>=3 */

    /* Compute the two squaring factors. */
    seq_t t1 = nm2 * nm2;                /* (n-2)^2 */
    seq_t t2 = nn * nn;                  /* n^2 */

    /* Compute the third multiplicand: 2*n^2-4*n+5+3*(-1)^n
       avoiding negative intermediates. */
    seq_t expr;
    if (n & 1) {                         /* odd n: 2*(n-1)^2 */
        seq_t nm1 = nn - 1;
        expr = (nm1 * nm1) * 2;
    } else {                             /* even n: 2*n^2-4*n+8 */
        seq_t temp = nn * nn;
        temp = temp * 2;                 /* 2*n^2 */
        expr = temp - (nn * 4) + 8;      /* -4*n + 8 */
    }

    /* Assemble the product and divide by 12. */
    seq_t prod = t1 * t2;                /* (n-2)^2 * n^2 */
    prod = prod * expr;                  /* times the third factor */
    prod = prod / (seq_t)12;             /* final division */

    return prod;
}
