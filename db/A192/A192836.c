/*
 * A192836  Molecular topological indices of the pan graphs.
 * https://oeis.org/A192836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (26 + 27*n + 4*n^2 + 2*n^3 + ((n%2==0)?(2+n):-(2+n))) / 4
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t to avoid implicit conversions */
    seq_t N = (seq_t)n;

    /* compute the polynomial terms */
    seq_t term1 = 26;                     /* constant term */
    seq_t term2 = 27 * N;                 /* linear term */
    seq_t n2    = N * N;                  /* n^2 */
    seq_t term3 = 4 * n2;                 /* quadratic term */
    seq_t n3    = n2 * N;                 /* n^3 */
    seq_t term4 = 2 * n3;                 /* cubic term */

    /* sum without the (-1)^n*(2+n) part */
    seq_t total = term1 + term2 + term3 + term4;

    /* add or subtract (2+n) depending on parity of n */
    if ((N & 1) == 0) {          /* even n */
        total += (2 + N);
    } else {                     /* odd n */
        total -= (2 + N);
    }

    /* divide by 4 (integer division truncates as required) */
    return total / 4;
}
