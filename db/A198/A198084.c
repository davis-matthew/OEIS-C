/*
 * A198084  Ceiling(n*sqrt(7)).
 * https://oeis.org/A198084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*sqrt(7))
 * Range:     u8 n <= 6, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    seq_t ans = 0;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid <= x / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t x = (seq_t)7 * nn * nn;
    seq_t r = isqrt(x);
    if (r * r < x) {
        ++r;
    }
    return r;
}
