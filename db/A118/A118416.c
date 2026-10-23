/*
 * A118416  Triangle read by rows: T(n,k) = (2*k-1)*2^(n-1), 0 < k <= n.
 * https://oeis.org/A118416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the smallest positive integer with r(r+1)/2 ≥ n. Let k
 *            = n – r(r–1)/2. Then a(n) = (2*k – 1) · 2^(r – 1) which equals
 *            T(r,k) from the given triangle
 * Range:     u8 n <= 19, u16 n <= 86, u32 n <= 394, u64 n <= 1743,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;                       /* row number */
    idx_t before = r * (r - 1) / 2;      /* terms before this row */
    idx_t k = n - before;                /* column number (1‑based) */

    seq_t odd_factor = (seq_t)(2 * k - 1);
    seq_t power_two  = pow2(r - 1);      /* 2^(r-1) */

    return odd_factor * power_two;
}
