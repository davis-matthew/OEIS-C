/*
 * A217893  50k^2-40k-17 interleaved with 50k^2+10k+13 for k=>0.
 * https://oeis.org/A217893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 50*(n/2)^2 - 40*(n/2) - 17 for even n, a(n) = 50*(n/2)^2
 *            + 10*(n/2) + 13 for odd n Sequence A217893 (interleaving of two
 *            quadratic sequences)
 * Range:     i8 n <= 4, i16 n <= 52, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* half = floor(n/2) */
    idx_t half = n >> 1;
    long long k = (long long)half;            /* k fits easily in long long for n <= 10000 */

    if (n & 1) { /* odd index */
        long long val = 50LL * k * k + 10LL * k + 13LL;
        return (seq_t)val;
    } else { /* even index */
        long long val = 50LL * k * k - 40LL * k - 17LL;
        return (seq_t)val;
    }
}
