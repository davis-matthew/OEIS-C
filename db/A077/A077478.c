/*
 * A077478  Rectangular array R read by antidiagonals: R(i,j) is the number of
 *          integers k that divide both i and j (i >= 1, j >= 1).
 * https://oeis.org/A077478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = τ(gcd(i,j)) where (i,j) is the nth element in the array
 *            read by antidiagonals; τ(m) is the number of divisors of m
 * Range:     u8 n <= 9847, u16 n <= 9847, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static inline seq_t divisor_count_u64(uint64_t n) {
    if (n == 0) return 0;
    seq_t cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

seq_t a(idx_t n) {
    /* Find the antidiagonal number t such that t*(t+1)/2 >= n. */
    idx_t t = 1;
    while ((__int128)t * (t + 1) / 2 < n) ++t;

    /* Position within the t‑th antidiagonal (0‑based). */
    idx_t prev = (__int128)t * (t - 1) / 2;
    idx_t pos  = n - prev - 1;

    /* Convert to 1‑based matrix coordinates (i,j). */
    idx_t i = pos + 1;
    idx_t j = t - pos;

    /* Compute τ(gcd(i,j)). */
    uint64_t g = gcd_u64(i, j);
    return divisor_count_u64(g);
}
