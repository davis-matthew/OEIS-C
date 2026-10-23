/*
 * A134577  Triangle read by rows: T(n,k) = tau(n/k)*k if k divides n, T(n,k)
 *          = 0 otherwise, where tau = A000005.
 * https://oeis.org/A134577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the position of n in the triangle.
 *            T(r,k) = k * tau(r/k) if k divides r, else 0. The algorithm: 1.
 *            Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2. 2. Column k
 *            = n - (r-1)*r/2. 3. If r % k != 0 return 0. 4. Compute m = r / k
 *            and tau(m) (number of divisors of m). 5...
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x) {
    /* integer square root using binary search with __int128 to avoid overflow */
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

static inline seq_t tau_u64(uint64_t n) {
    /* divisor count for n (produces a value that fits in seq_t) */
    seq_t cnt = 1;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (x > 1) {
        cnt *= (seq_t)2;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence starts at index 1; safe guard */
    /* Find the row r with (r-1)*r/2 < n <= r*(r+1)/2 using binary search. */
    idx_t low = 0, high = 1;
    /* increase upper bound until it covers n */
    while ( ( (__int128)high * (high + 1) ) / 2 < n ) {
        high <<= 1;
    }
    /* binary search for minimum high satisfying condition */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n ) {
            high = mid;
        } else {
            low = mid;
        }
    }
    idx_t r = high;

    /* column k */
    idx_t prev = ( ( (__int128)(r - 1) * r ) ) / 2;
    idx_t k = n - prev;   /* 1 <= k <= r */

    if (r % k != 0) {
        return 0;        /* T(r,k)=0 when k does not divide r */
    }

    idx_t m = r / k;                  /* m = r/k */
    seq_t tau = tau_u64((uint64_t)m); /* divisor count */

    seq_t result = (seq_t)k * tau;     /* T(r,k) = k * tau(m) */
    return result;
}
