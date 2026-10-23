/*
 * A167471  Janet periodic table of the elements and structured hexagonal
 *          diamond numbers. a(n) = A166911(2*n) + A166911(2*n+1).
 * https://oeis.org/A167471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n*(2 - 3*n + 4*n^2)/3
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 586, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = 16 * n * ((4*n*n - 3*n + 2) / 3)  */
/*           = (16*n/3) * (4*n*n - 3*n + 2)   when n % 3 == 0   */
/*           = 16*n * ((4*n*n - 3*n + 2)/3)   otherwise           */

seq_t a(idx_t n)
{
    /* cast index to sequence type */
    seq_t nn = (seq_t)n;

    /* compute 4*n*n - 3*n + 2  */
    seq_t N = nn * nn;          /* n^2 */
    N = 4 * N;                  /* 4*n^2 */
    N -= 3 * nn;                /* -3*n */
    N += 2;                     /* +2 */

    seq_t result;

    if (nn % 3 == 0) {
        /* 16*n is divisible by 3 when n % 3 == 0 */
        seq_t t = nn / 3;       /* n/3 */
        t = t * 16;             /* 16*(n/3) */
        result = t * N;         /* (16*n/3) * N */
    } else {
        /* 4*n*n - 3*n + 2 is divisible by 3 otherwise */
        N = N / 3;              /* N/3 */
        result = 16 * nn * N;   /* 16*n * (N/3) */
    }

    return result;
}
