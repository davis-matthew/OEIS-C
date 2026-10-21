/*
 * A059329  Number of 3 X 3 matrices, with elements from {0,...,n}, having the
 *          property that the middle element of each of the eight 3-element
 *          horizontal, vertical and diagonal lines equals the average of the
 *          two end elements.
 * https://oeis.org/A059329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1+n)*(9+3*(-1)^n+4*n+2*n^2))/12
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (n+1)*(12+4*n+2*n^2)/12          if n even */
/*          = (n+1)*(6+4*n+2*n^2)/12           if n odd  */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N & 1U) {                          /* odd n */
        seq_t k = N >> 1;                  /* k = (n-1)/2   */
        /* compute (n+1)*(6+4*n+2*n^2)/12 = 2*(k+1)*(3+4k+2k^2)/3 */
        seq_t t = 3 + 4 * k + 2 * k * k;   /* 3+4k+2k^2   */
        seq_t prod = 2 * (k + 1) * t;      /* numerator   */
        return prod / 3;                   /* final division */
    } else {                               /* even n */
        /* compute (n+1)*(12+4*n+2*n^2)/12 */
        seq_t d = 12 + 4 * N + 2 * N * N;  /* 12+4n+2n^2 */
        seq_t prod = (N + 1) * d;          /* numerator   */
        return prod / 12;                  /* final division */
    }
}
