/*
 * A232091  Smallest square or promic (oblong) number greater than or equal to
 *          n.
 * https://oeis.org/A232091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n / ceil(sqrt(n))) * ceil(sqrt(n))
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_isqrt_u64(idx_t n)
{
    /* Compute floor(sqrt(n)) using binary search with division to avoid overflow. */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            lo = 1;          /* avoid division by zero on the next check */
            continue;
        }
        if (mid <= n / mid) {  /* mid*mid <= n  */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 0 */
        return (seq_t)0;

    idx_t root = floor_isqrt_u64(n);          /* floor(sqrt(n)) */
    /* ceil_sqrt is floor_sqrt when n is a perfect square, otherwise +1 */
    idx_t ceil_sqrt = (root * root == n) ? root : root + 1;

    /* ceil(n / ceil_sqrt) as integer arithmetic */
    idx_t q = (n + ceil_sqrt - 1) / ceil_sqrt;

    /* final result; seq_t handles any necessary promotion or overflow */
    return (seq_t)q * (seq_t)ceil_sqrt;
}
