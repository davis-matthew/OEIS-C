/*
 * A184808  n + floor(r*n), where r = sqrt(2/3); complement of A184809.
 * https://oeis.org/A184808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(r*n), where r = sqrt(2/3)
 * Range:     u8 n <= 6, u16 n <= 139, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t
isqrt_seq_t(seq_t x)
{
    seq_t low  = 0;
    seq_t high = 1;
    /* Find an upper bound high such that high*high > x */
    while (high <= x / high)
        high <<= 1;
    /* Binary search for the floor of sqrt(x) */
    while (low + 1 < high) {
        seq_t mid = low + (high - low) / 2;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Return 0 for n == 0 (not part of the OEIS offset) */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;

    /* Compute floor(sqrt(6 * n^2)) */
    seq_t sq = nn * nn;    /* n^2   (may overflow if n too large, but
                               n is assumed small enough so that
                               the result fits in seq_t) */
    sq = sq * (seq_t)6;    /* 6 * n^2 */

    seq_t sqrt6n2 = isqrt_seq_t(sq);

    /* floor(r*n) = floor(sqrt(6)*n / 3) = floor(sqrt(6*n^2)/3) */
    seq_t floor_rn = sqrt6n2 / (seq_t)3;

    /* a(n) = n + floor(r*n) */
    return nn + floor_rn;
}
