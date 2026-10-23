/*
 * A147658  (1, 2, -4, 6, -8, ...) interleaved with (3, -3, 3, -3, 3, ...).
 * https://oeis.org/A147658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = 1; for k >= 1: a(2k) = 3 * (-1)^(k-1) a(2k+1) = 2 *
 *            (-1)^(k-1) * k
 * Range:     i8 n <= 130, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based */
    if (n == 1) {
        return (seq_t)1;
    }

    /* n is even: 2k */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;              /* k = n/2 */
        seq_t sign = (k & 1ULL) ? 1 : -1; /* (-1)^(k-1) */
        return 3 * sign;
    }

    /* n is odd: 2k+1, n >= 3 */
    idx_t k = (n - 1) >> 1;            /* k = (n-1)/2 */
    seq_t sign = (k & 1ULL) ? 1 : -1;  /* (-1)^(k-1) */
    return 2 * sign * (seq_t)k;
}
