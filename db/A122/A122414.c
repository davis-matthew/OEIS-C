/*
 * A122414  Triangle T(n,k) for 1 <= k <= n read by rows, where T(n,k) = 1 if
 *          gcd(n,k) is prime, 0 otherwise.
 * https://oeis.org/A122414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if gcd(row(n), col(n)) is prime, otherwise 0 in the
 *            triangular array read by rows
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

/* Return the triangular number T(r) = r*(r+1)/2 using 128‑bit arithmetic to avoid overflow. */
static inline __int128 tri_sum(idx_t r) {
    return (__int128)r * (r + 1) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined below offset; return 0 */

    /* Find r such that T(r-1) < n <= T(r) */
    idx_t low = 1, high = 2;
    while (tri_sum(high) < ( __int128)n) high <<= 1;  /* exponential search */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (tri_sum(mid) >= ( __int128)n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = (r > 1) ? (idx_t)tri_sum(r - 1) : 0;
    idx_t k = n - prev;

    seq_t g = gcd((seq_t)r, (seq_t)k);
    if (g <= 1) return 0;
    return is_prime((idx_t)g) ? 1 : 0;
}
