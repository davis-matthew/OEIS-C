/*
 * A181538  T(n, k) = sum_(1 <= j <= k) [j | k] j mu(k / j) gcd(n, k),
 *          triangle read by rows.
 * https://oeis.org/A181538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(row, col) * φ(col), where row and col satisfy n =
 *            row*(row-1)/2 + col and 1 ≤ col ≤ row
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

static inline uint64_t tri(uint64_t r) {
    /* Compute r*(r+1)/2 using 128‑bit intermediate to avoid overflow. */
    return (uint64_t)(((unsigned __int128)r * (r + 1)) / 2);
}

static inline uint64_t phi_uint64(uint64_t n) {
    if (n <= 1) return n;
    uint64_t res = n;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            res -= res / p;
        }
    }
    if (n > 1) res -= res / n;
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Find the smallest row r with tri(r) >= n. */
    uint64_t low = 1, high = 1;
    while (tri(high) < n) high <<= 1;   /* safe for n <= 10^4 */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (tri(mid) >= n) high = mid;
        else low = mid + 1;
    }
    uint64_t row = low;
    uint64_t col = n - tri(row - 1);

    uint64_t g = gcd_uint64(row, col);
    uint64_t phi = phi_uint64(col);
    return (seq_t)g * (seq_t)phi;
}
