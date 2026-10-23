/*
 * A131423  a(n) = n*(n+2)*(2*n-1)/3. Also, row sums of triangle A131422.
 * https://oeis.org/A131423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*(2*n-1)/3
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res;

    /* Divide one of the factors by 3 to reduce overflow. */
    if (N % 3 == 0) {
        res = (N / 3) * (N + 2) * ((2 * N) - 1);
    } else if ((N + 2) % 3 == 0) {
        res = N * ((N + 2) / 3) * ((2 * N) - 1);
    } else {
        /* (2*N-1) is always odd, but will be divisible by 3 for some n. */
        res = N * (N + 2) * (((2 * N) - 1) / 3);
    }

    return res;
}
