/*
 * A066704  Triangle with a(n,k) = C(n,floor(n/k)) with n>=k>=1.
 * https://oeis.org/A066704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r, ⌊r/k⌋), * where r = smallest integer with
 *            r(r+1)/2 ≥ n, * and k = n – r(r‑1)/2. Thus the sequence lists
 *            the * triangle entries a(r, k) = binom(r, ⌊r/k⌋) row by row
 * Range:     u8 n <= 29, u16 n <= 172, u32 n <= 596, u64 n <= 2279,
 *            u128 n <= 8647, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset 1: n ≥ 1, return 0 for safety */
        return 0;

    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t low  = 1;
    idx_t high = n;            /* row cannot exceed n for n ≥ 1 */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        idx_t total = (mid * (mid + 1)) / 2;
        if (total >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index k within the row. */
    idx_t prev = (r - 1) * r / 2;   /* entries in previous rows */
    idx_t k = n - prev;             /* 1 ≤ k ≤ r */

    /* Compute the triangle entry a(r,k) = binom(r, floor(r/k)). */
    idx_t t = r / k;                /* floor division */
    return binom(r, t);
}
