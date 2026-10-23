/*
 * A038390  Bisection of A028289.
 * https://oeis.org/A038390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n^2+2+floor(n/3)*(10*floor(n/3)^2-(8*n-15)*floor(n/3)+2*n^2-8*n+7))/2
 * Range:     u8 n <= 9, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* floor division by 3 */
    seq_t k = (seq_t)(n / 3);

    /* base term: 2*n^2 + 2 */
    seq_t result = 2 * N * N + 2;

    if (k != 0) {
        /* compute inner part: 10*k^2 - (8*N-15)*k + 2*N^2-8*N+7 */
        seq_t k2   = k * k;
        seq_t t1   = 10 * k2;
        seq_t t2   = (8 * N - 15) * k;
        seq_t t3   = 2 * N * N - 8 * N + 7;
        seq_t inner = t1 + t3 - t2;

        /* add the product k * inner */
        result += k * inner;
    }

    /* divide by 2 (the numerator is always even) */
    return result / 2;
}
