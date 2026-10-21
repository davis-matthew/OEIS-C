/*
 * A056847  Nearest integer to n - sqrt(n).
 * https://oeis.org/A056847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - floor(sqrt(n)+1/2)
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned long long floor_sqrt_uint64(unsigned long long x)
{
    unsigned long long low = 0, high = 0xFFFFFFFFULL, ans = 0;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    unsigned long long s = floor_sqrt_uint64((unsigned long long)n);
    unsigned long long t = (unsigned long long)((__int128)s * s + s + 1);
    unsigned long long k = ((unsigned long long)n >= t) ? s + 1 : s;
    return (seq_t)((unsigned long long)n - k);
}
