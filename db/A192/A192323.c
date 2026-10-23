/*
 * A192323  Expansion of theta_3(q^3) * theta_3(q^5) in powers of q.
 * https://oeis.org/A192323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k,l≥0} (k==0?1:2)*(l==0?1:2) where 3·k² + 5·l² = n
 *            (coefficients of the product θ₃(q³)·θ₃(q⁵) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: the greatest integer x with x² ≤ n.
   Works for 0 ≤ n < 2⁶⁴ and uses only integer arithmetic. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid) {                /* mid² ≤ n  (overflow‑free) */
            if (mid + 1 <= n / (mid + 1))
                low = mid + 1;                /* try a larger square */
            else
                return mid;                   /* exact or best fit */
        } else {
            high = mid - 1;                   /* too large */
        }
    }
    return low - 1;                           /* should not happen */
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t k = 0; ; ++k) {
        idx_t k2    = k * k;
        idx_t term3 = 3 * k2;
        if (term3 > n)
            break;

        idx_t rem = n - term3;
        if (rem % 5 != 0)
            continue;

        idx_t l2 = rem / 5;            /* candidate for l² */
        idx_t l  = isqrt_idx(l2);
        if (l * l != l2)
            continue;                  /* not a perfect square */

        if (k == 0 && l == 0)
            count += 1;
        else if (k == 0 || l == 0)
            count += 2;
        else
            count += 4;
    }

    return count;
}
