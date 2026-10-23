/*
 * A192032  Square array read by antidiagonals: W(m,n) (m >= 0, n >= 0) is the
 *          Wiener index of the graph G(m,n) obtained in the following way:
 *          connect by an edge the center of an m-edge star with the center of
 *          an n-edge star. The Wiener index of a connected graph is the sum
 *          of distances between all unordered...
 * https://oeis.org/A192032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term of the square array W(m,n) read by
 *            antidiagonals. For each n >= 0: let d = floor((sqrt(8*n + 1) -
 *            1)/2) // diagonal index (m+n) let prev = d*(d+1)/2 // elements
 *            before this diagonal let r = n - prev // offset on the diagonal
 *            let m = r // row index let c = d - r // column index a(...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value.
   Returns the largest x such that x*x <= val. */
static inline idx_t
isqrt128(unsigned __int128 val)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;   /* maximum 64‑bit unsigned value */
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= val) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Locate the diagonal */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t sqrt_t = isqrt128(t);
    idx_t d = (sqrt_t - 1) / 2;

    /* Position inside the diagonal */
    idx_t prev = d * (d + 1) / 2;
    idx_t r = n - prev;
    idx_t m = r;
    idx_t c = d - r;

    /* Compute the Wiener index formula */
    seq_t M = (seq_t)m;
    seq_t C = (seq_t)c;
    return M * M + C * C + (seq_t)3 * M * C + (seq_t)2 * M + (seq_t)2 * C + 1;
}
