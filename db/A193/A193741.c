/*
 * A193741  Mirror of the triangle A193740.
 * https://oeis.org/A193741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r - c), where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2. w(r,k) is defined by the triangle A193740: w(r,0)=1;
 *            w(r,k)=binom(k+3,3) for k <= r-2, r>=2 w(r,k)=binom(r+2,3)-1 for
 *            k = r-1 or k = r (r>=2) Base: w(0,0)=1, w(1,0)=w(1,1)=1
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Edge cases n == 0 or 1 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Compute r = floor((sqrt(8*n+1)-1)/2) */
    seq_t tmp = (seq_t)8 * (seq_t)n + 1;
    seq_t s = floor_sqrt(tmp);
    idx_t r = (idx_t)((s - 1) / 2);

    /* Start index of row r */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;          /* column within the row (0‑based) */
    idx_t k = r - c;              /* index in the mirrored triangle */

    /* Handle rows 0 and 1 explicitly */
    if (r == 0) return (seq_t)1;          /* only k = 0 */
    if (r == 1) return (seq_t)1;          /* k = 0 or 1 */

    /* For r >= 2 */
    if (k <= r - 2) {
        /* w(r,k) = binom(k+3,3) */
        seq_t kk = (seq_t)k + 3;
        return kk * (kk - 1) * (kk - 2) / 6;
    } else {
        /* k == r-1 or k == r */
        seq_t rr = (seq_t)r + 2;
        return rr * (rr - 1) * (rr - 2) / 6 - 1;
    }
}
