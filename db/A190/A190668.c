/*
 * A190668  a(n) = Product_{k>=1} floor(n^(1/k)).
 * https://oeis.org/A190668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k>=1} floor(n^(1/k))
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer k‑th root, floor of n^(1/k), for n≥1, k≥1 */
static idx_t _floor_kth_root(idx_t n, idx_t k)
{
    idx_t low = 1;
    idx_t high = n;          /* a^k never exceeds n, so a ≤ n */

    while (low < high) {
        idx_t mid = (low + high + 1) / 2;

        /* Check if mid^k ≤ n, with overflow safe comparison */
        __int128 res = 1;
        bool ok = true;
        for (idx_t i = 0; i < k; ++i) {
            res *= (__int128)mid;
            if (res > (__int128)n) { ok = false; break; }
        }

        if (ok)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* not used, but defined for safety */

    seq_t product = (seq_t)1;
    for (idx_t k = 1;; ++k) {
        idx_t root = _floor_kth_root(n, k);
        if (root <= 1) break;          /* remaining terms would be 1 */
        product *= (seq_t)root;
    }
    return product;
}
