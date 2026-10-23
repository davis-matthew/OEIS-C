/*
 * A189135  Zero-one sequence based on the central polygonal numbers n^2-n+1:
 *          a(A002061(k))=a(k); a(A135668(k))=1-a(k), a(1)=0.
 * https://oeis.org/A189135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 0 * Let c(n) = floor((1 + sqrt(4n-3))/2) (number of
 *            central polygonal numbers ≤ n). * If n is a central polygonal
 *            number (i.e. sqrt(4n-3) is integer and odd), * a(n) = a(c(n)) //
 *            keep same value * otherwise * a(n) = 1 – a(n - c(n)) // flip
 *            value * The algorithm iterates until n becomes 1...
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for uint64_t using binary search. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    bool flip = false;

    while (n > 1) {
        /* discriminant = 4*n - 3; use 128‑bit arithmetic to avoid overflow. */
        idx_t disc = (idx_t)(((__int128)4 * n) - 3);
        idx_t sqrtD = isqrt_u64(disc);

        /* cnt = floor((1 + sqrtD) / 2) = number of central numbers ≤ n */
        idx_t cnt = (1 + sqrtD) >> 1;

        /* Check if n is a central polygonal number:
         *   sqrtD must be integer and odd. */
        if ((sqrtD * sqrtD == disc) && ((1 + sqrtD) & 1) == 0) {
            /* central case: a(n) = a(cnt) */
            n = cnt;
        } else {
            /* non‑central case: a(n) = 1 - a(n - cnt) */
            n -= cnt;
            flip = !flip;
        }
    }

    return flip ? (seq_t)1 : (seq_t)0;
}
