/*
 * A129353  Triangle read by rows: A051731 * A115361 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A129353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = * Let r be the smallest integer with r*(r+1)/2 ≥ n (row
 *            index). * Let k = n - (r-1)*r/2 (column index). * If r % k ≠ 0
 *            then a(n) = 0. * Else a(n) = exponent of 2 in (r/k) + 1, i.e. *
 *            a(n) = 1 + v₂(r/k). * This follows from T(n,k) = A001511(n/k) =
 *            v₂(n/k)+1 when k|n
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   triangular_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangular_u128.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index < 1 is undefined, return 0 for safety */

    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 0;
    while (triangular_u128(r) < (unsigned __int128)n) ++r;

    /* Compute column k (1‑based). */
    unsigned __int128 prev_tri = triangular_u128(r - 1);
    idx_t k = n - (idx_t)prev_tri;   /* k ∈ [1, r] */

    /* If k does not divide r, the value is 0. */
    if (r % k != 0) return 0;

    /* Compute m = r / k and count trailing zeros (i.e. exponent of 2). */
    idx_t m = r / k;
    uint64_t m64 = (uint64_t)m;          /* m fits in 64 bits (r ≤ 10⁶ for n≈10⁵) */
    int exp = 0;
    while ((m64 & 1ULL) == 0) {
        ++exp;
        m64 >>= 1;
    }

    return (seq_t)(exp + 1);
}
