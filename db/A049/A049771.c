/*
 * A049771  Triangular array T read by rows: T(n,k) = (k^4 mod n) + (n^4 mod
 *          k).
 * https://oeis.org/A049771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element (row‑major) of the triangular array T
 *            where T(n,k) = (k^4 mod n) + (n^4 mod k), with 1 ≤ k ≤ n
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   mod_pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow4.h"

/* find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2 */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = 1;
    /* expand high until enough */
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour for n < 1 is irrelevant */

    /* locate row r and position k */
    idx_t r = row_of(n);
    idx_t prev_tri = ((r - 1) * r) / 2;   /* number of elements before row r */
    idx_t k = n - prev_tri;               /* 1‑based column index */

    /* compute T(r,k) = k^4 mod r + r^4 mod k */
    seq_t t1 = mod_pow4(k, r);   /* k^4 mod r */
    seq_t t2 = mod_pow4(r, k);   /* r^4 mod k */

    return t1 + t2;
}
