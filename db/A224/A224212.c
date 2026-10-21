/*
 * A224212  Number of nonnegative solutions to x^2 + y^2 <= n.
 * https://oeis.org/A224212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y) ∈ ℕ² : x² + y² ≤ n }|
 * Range:     u8 n <= 304, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a non‑negative 64‑bit value.
   Returns ⌊√n⌋.  */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low  = 1;
    idx_t high = (idx_t)0xFFFFFFFFULL;   /* floor(√(2⁶⁴-1)) = 2³²-1 */
    idx_t ans  = 1;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* avoid division by zero; mid ≥ 1 here */
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* safety guard, should not happen */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t xmax = isqrt_u64(n);
    seq_t total = 0;

    for (idx_t x = 0; x <= xmax; ++x) {
        idx_t x2  = x * x;          /* x² fits in 64 bits */
        idx_t rem = n - x2;         /* remaining radius² */
        idx_t ymax = isqrt_u64(rem);
        total += (seq_t)(ymax + 1); /* y = 0 … ymax, inclusive */
    }
    return total;
}
