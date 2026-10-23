/*
 * A135459  Numbers n such that the difference between sqrt(2)*n and the
 *          nearest integer is smaller than 1/10.
 * https://oeis.org/A135459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that |k*sqrt(2) -
 *            nearest integer| < 0.1
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static uint64_t isqrt_uint128(__int128 x) {
    uint64_t low = 0, high = UINT64_MAX, mid;
    uint64_t res = 0;

    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 square = (__int128)mid * mid;
        if (square <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* Tests whether integer k satisfies the criterion. */
static bool good_k(uint64_t k) {
    __int128 M = (__int128)2 * k * k;          /* M = 2*k^2 */
    uint64_t a = isqrt_uint128(M);             /* a = floor(sqrt(M)) */

    __int128 lhs  = (__int128)100 * M;
    __int128 rhs1 = (__int128)100 * a * a + (__int128)20 * a + 1;
    if (lhs < rhs1) return true;               /* distance < 0.1 from below */

    __int128 rhs2 = (__int128)100 * a * a + (__int128)180 * a + 81;
    if (lhs > rhs2) return true;               /* distance < 0.1 from above */

    return false;
}

/* Return the n-th term of A135459 (1‑based indexing). */
seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;

    while (1) {
        if (good_k(k)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
