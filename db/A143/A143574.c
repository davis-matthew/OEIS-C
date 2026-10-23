/*
 * A143574  Sum of all distinct squares occurring when partitioning n into two
 *          squares.
 * https://oeis.org/A143574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*A010052(k)*A010052(n-k)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v, res = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        if (mid > v / mid) {
            high = mid - 1;
        } else {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t i = 0; i <= sqrt_n; ++i) {
        idx_t sqi = i * i;
        idx_t rem = n - sqi;
        idx_t sqr = isqrt_u64(rem);
        if (sqr * sqr == rem) {
            total += (seq_t)sqi;
        }
    }

    return total;
}
