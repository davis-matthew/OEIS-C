/*
 * A211312  Square array of Delannoy numbers D(i,j) mod 3 (i >= 0, j >= 0)
 *          read by antidiagonals.
 * https://oeis.org/A211312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=0}^{n} binomial(k, j) * binomial(n-j, k) (mod 3)
 *            which equals D(i,j) mod 3 where (i,j) is the point read by
 *            antidiagonals
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   delannoy_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/delannoy_mod3.h"

seq_t a(idx_t n) {
    /* Find the anti‑diagonal d so that n lies on it.  The total number
       of elements up to and including diagonal d is (d+1)(d+2)/2. */
    idx_t d = 0;
    for (;; ++d) {
        idx_t total = (d + 1) * (d + 2) / 2;
        if (n < total)
            break;
    }
    /* Position of n within this diagonal (0‑based), which equals i. */
    idx_t pos = n - d * (d + 1) / 2;
    idx_t i = pos;
    idx_t j = d - pos;
    return delannoy_mod3(i, j);
}
