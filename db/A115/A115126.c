/*
 * A115126  First (k=1) triangle of numbers related to totally asymmetric
 *          exclusion process (case alpha=1, beta=1).
 * https://oeis.org/A115126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r+m, r) * (r-m+1) / (r+1) where r is the unique
 *            integer with r*(r+1)/2 ≥ n and m = n - (r-1)*r/2 (1 ≤ m ≤ r)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Find r such that T_r = r*(r+1)/2 ≥ n.
       Binary search on 1 .. n (r ≤ n). Use 128‑bit
       arithmetic to avoid overflow for very large n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index m within the row. */
    idx_t prev = (r - 1) * r / 2;
    idx_t m = n - prev;                 /* 1 ≤ m ≤ r */

    /* Triangle value. */
    seq_t val = binomial(r + m, r);
    val *= (seq_t)(r - m + 1);
    val /= (seq_t)(r + 1);
    return val;
}
