/*
 * A143131  Binomial transform of [1, 4, 10, 20, 0, 0, 0, ...].
 * https://oeis.org/A143131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-39 + 77*n - 45*n^2 + 10*n^3)/3
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* outside the defined offset */

    seq_t result = 1;                   /* s0 * C(n-1,0) = 1 */

    if (n > 1) {                        /* add s1 * C(n-1,1) = 4*(n-1) */
        seq_t t = (seq_t)n - 1;
        result += t * 4;
    }

    if (n > 2) {                        /* add s2 * C(n-1,2) = 10*(n-1)(n-2)/2 */
        seq_t t1 = (seq_t)n - 1;
        seq_t t2 = (seq_t)n - 2;
        seq_t t = (t1 * t2) / 2;
        result += t * 10;
    }

    if (n > 3) {                        /* add s3 * C(n-1,3) = 20*(n-1)(n-2)(n-3)/6 */
        seq_t t1 = (seq_t)n - 1;
        seq_t t2 = (seq_t)n - 2;
        seq_t t3 = (seq_t)n - 3;
        seq_t t = (t1 * t2 * t3) / 6;
        result += t * 20;
    }

    return result;
}
