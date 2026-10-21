/*
 * A217463  a(n) is the sum of total number of positive integer solutions to
 *          each of a^2 + b^2 = n, a^2 + 2*b^2 = n, a^2 + 3*b^2 = n and a^2 +
 *          7*b^2 = n. (Order does not matter for the equation a^2+b^2 = n).
 * https://oeis.org/A217463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A217463
 *  a(n) = number of positive integer solutions to
 *         a^2 + b^2  = n  (unordered),
 *         a^2 + 2*b^2 = n  (ordered),
 *         a^2 + 3*b^2 = n  (ordered),
 *         a^2 + 7*b^2 = n  (ordered)
 *
 *  The implementation uses only integer arithmetic and an inline
 *  integer square‑root routine to avoid external dependencies.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = ((idx_t)1 << 62);  /* largest power of four <= n */

    /* Adjust starting bit to be <= n */
    while (bit > n) bit >>= 2;

    while (bit != 0) {
        idx_t tmp = res + bit;
        if (tmp <= n / tmp) {
            res = tmp;
        }
        bit >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* ---------- k = 1 : unordered a^2 + b^2 = n ---------- */
    if (n >= 2) {
        idx_t max_a = isqrt_u64(n / 2);  /* floor(sqrt(n/2)) */
        for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
            idx_t r = n - a_val * a_val;          /* remaining part for b^2 */
            idx_t b = isqrt_u64(r);
            if (b * b == r && b >= a_val) {      /* b^2 exactly r and unordered */
                total += (seq_t)1;
            }
        }
    }

    /* ---------- k = 2, 3, 7 : ordered a^2 + k*b^2 = n ---------- */
    const idx_t ks[3] = {2, 3, 7};
    for (int i = 0; i < 3; ++i) {
        idx_t k = ks[i];
        if (n <= k) continue;                  /* need b >= 1 => r >= k */
        idx_t max_a = isqrt_u64(n - k);         /* a^2 <= n - k ensures r >= k */
        for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
            idx_t r = n - a_val * a_val;
            if (r % k != 0) continue;
            idx_t b2 = r / k;
            idx_t b  = isqrt_u64(b2);
            if (b * b == b2) {                  /* b^2 exactly divides r/k */
                total += (seq_t)1;
            }
        }
    }

    return total;
}
