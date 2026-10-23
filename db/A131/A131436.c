/*
 * A131436  Triangle read by rows, (n-1) zeros followed by 2^n - 1.
 * https://oeis.org/A131436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 unless n is a triangular number T_r = r(r+1)/2. In that
 *            case a(n) = 2^r - 1
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that T_r >= n.  T_r = r(r+1)/2. */
    for (idx_t r = 1; ; ++r) {
        __int128 t = (__int128)r * (r + 1) / 2;   /* triangular number */
        if (t >= n) {
            if (t == n) {                      /* at the end of the row */
                /* Compute 2^r - 1.  Shift only when it is safe. */
                if (r < (sizeof(seq_t) * 8)) {
                    return ((seq_t)1 << r) - 1;
                } else {
                    /* For large r, use pow2 (works for arbitrary‑precision). */
                    seq_t v = pow2(r);
                    return v - 1;
                }
            } else {
                return 0;                       /* not a triangular index */
            }
        }
    }
}
