/*
 * A162623  Triangle read by rows in which row n lists n terms, starting with
 *          n, such that the difference between successive terms is equal to
 *          n^4 - 1 = A123865(n).
 * https://oeis.org/A162623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   For n ≥ 1, let r be the unique integer with * (r‑1)*r/2 + 1 ≤ n
 *            < r*(r+1)/2 + 1. * Let k = n – ((r‑1)*r/2 + 1). The n‑th term is
 *            * a(n) = r + k·(r⁴ – 1). This implements the definition of *
 *            OEIS A162623
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   offset128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/offset128.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* out‑of‑range safety, not required by OEIS */

    /* Find the row r via binary search on r */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (offset128(mid) <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Position within the row */
    idx_t offset_r = (idx_t)offset128(r);
    idx_t k = n - offset_r;

    /* Compute r⁴ – 1 as seq_t */
    seq_t sr = (seq_t)r;
    seq_t d = sr * sr * sr * sr - 1;

    /* a(n) = r + k·(r⁴ – 1) */
    return sr + (seq_t)k * d;
}
