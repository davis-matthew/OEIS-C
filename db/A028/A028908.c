/*
 * A028908  Arrange digits of squares in descending order.
 * https://oeis.org/A028908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digits of n^2 sorted in descending order
 * Range:     u8 n <= 11, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 in seq_t.  Use explicit casts from idx_t. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Count decimal digits of sq. */
    unsigned char cnt[10] = {0};
    while (sq > 0) {
        /* The remainder is guaranteed to fit in a native integer. */
        unsigned d = (unsigned)(sq % 10);
        cnt[d]++;
        sq /= 10;
    }

    /* Reconstruct the integer from the digit counts in descending order. */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        unsigned times = cnt[d];
        while (times--) {
            result = result * 10 + d;
        }
    }

    return result;
}
