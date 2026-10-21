/*
 * A212020  Number of (w,x,y,z) with all terms in {1,...,n} and w*x=3*y*z.
 * https://oeis.org/A212020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4-tuples (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and w·x
 *            = 3·y·z The implementation uses a two‑phase counting: 1) Count
 *            ordered pairs (y,z) for each product m ≤ ⌊n²/3⌋ and store the
 *            counts in an array F[m]. 2) For each ordered pair (w,x) with
 *            product t, add F[t/3] to the answer (when 3 di...
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases immediately */
    if (n < 3)
        return 0;

    /* lim = floor(n*n / 3) – largest product m = y*z that matters */
    idx_t lim = (n * n) / 3;

    /* Allocate array F[0..lim] of 16‑bit counts */
    uint16_t *F = (uint16_t *)calloc((size_t)lim + 1, sizeof(uint16_t));
    if (!F)   /* out‑of‑memory – return 0 (unlikely for n ≤ 10000) */
        return 0;

    /* Phase 1: count pairs (y,z) and fill F */
    for (idx_t y = 1; y <= n && y <= lim; ++y) {
        idx_t max_z = lim / y;
        if (max_z > n) max_z = n;
        for (idx_t z = 1; z <= max_z; ++z) {
            size_t m = y * z;          /* m ≤ lim by construction */
            ++F[m];
        }
    }

    /* Phase 2: count pairs (w,x) and accumulate the answer */
    seq_t ans = 0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t t = w * x;
            if (t % 3 != 0)
                continue;
            size_t m = t / 3;
            if (m <= lim)
                ans += (seq_t)F[m];
        }
    }

    free(F);
    return ans;
}
