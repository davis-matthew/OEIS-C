/*
 * A080883  Distance of n to next square.
 * https://oeis.org/A080883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n) + 1)^2 - n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute floor(sqrt(n)) by binary search */
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    idx_t sqrt_floor = lo;         /* floor(sqrt(n)) */
    seq_t m = (seq_t)(sqrt_floor + 1);  /* next integer after the square root */
    seq_t sq = m * m;               /* (floor(sqrt(n))+1)^2 */
    return sq - (seq_t)n;          /* distance to next square */
}
