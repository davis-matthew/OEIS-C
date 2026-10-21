/*
 * A211339  Number of integer pairs (x,y) such that 1 < x <= y <= n and x^2 +
 *          y^2 <= n.
 * https://oeis.org/A211339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{x=1}^{⌊√(n/2)⌋} (⌊√(n - x²)⌋ - x + 1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts integer pairs 1 ≤ x ≤ y ≤ n with x² + y² ≤ n. */

#include <stdint.h>

/* integer square‑root of a 64‑bit unsigned integer */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n, mid, root = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (mid == 0) { lo = 1; continue; }
        /* use 128‑bit to avoid overflow */
        if ((unsigned __int128)mid * mid <= n) {
            root = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return root;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* no pairs for n = 0 or 1 */
    idx_t m = int_isqrt(n / 2);              /* upper bound for x */
    seq_t count = 0;
    for (idx_t x = 1; x <= m; ++x) {
        unsigned __int128 x2 = (unsigned __int128)x * x;
        idx_t rem   = n - (idx_t)x2;          /* n - x²  (safe because x ≤ m) */
        idx_t y_max = int_isqrt(rem);        /* floor(√(n - x²)) */
        count += (seq_t)(y_max - x + 1);      /* y runs from x to y_max */
    }
    return count;
}
