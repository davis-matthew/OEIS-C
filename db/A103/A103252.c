/*
 * A103252  Array A000292(n)*A000217(k), read by antidiagonals.
 * https://oeis.org/A103252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A000292(r) * A000217(c) where the array A000292(i) *
 *            A000217(j) is read by antidiagonals. For a given n: let d =
 *            smallest integer with d*(d+1)/2 >= n (diagonal index, 1‑based)
 *            let t = n - (d-1)*d/2 - 1 (offset in diagonal, 0‑based) then r =
 *            d - t, c = t + 1 result = C(r+2,3) * C(c+1,2)
 * Range:     u8 n <= 30, u16 n <= 386, u32 n <= 8568, u64 n <= 8568,
 *            u128 n <= 8568, big any n
 * Helpers:   tetrahedral
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/tetrahedral.h"

/* helper: triangular number C(c+1,2) */
static seq_t triangular(idx_t c) {
    return ((seq_t)c * (c + 1)) / 2;
}

seq_t a(idx_t n) {
    /* locate diagonal d */
    idx_t d;
    long double approx = (sqrtl(8.0L * n + 1.0L) - 1.0L) / 2.0L;
    d = (idx_t)approx;
    /* adjust to guarantee d satisfies d*(d+1)/2 >= n */
    while (((__int128)d * (d + 1)) / 2 < n) d++;
    while (d > 0 && (((__int128)(d - 1) * d) / 2) >= n) d--;
    /* offset within the diagonal */
    idx_t prev = (((__int128)(d - 1) * d) / 2);
    idx_t t = n - prev - 1;            /* 0‑based within diagonal */
    idx_t r = d - t;
    idx_t c = t + 1;
    return tetrahedral(r) * triangular(c);
}
