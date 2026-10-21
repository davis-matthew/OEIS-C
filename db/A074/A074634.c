/*
 * A074634  Cototient-remainder triangle: triangular array T(n,k) (n >= 1, 0
 *          <= k < n) read by rows, where T(n,k) = smallest number x such that
 *          cototient(x) mod n = k.
 * https://oeis.org/A074634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = smallest x such that cototient(x) mod r = k, where r =
 *            smallest integer with r*(r+1)/2 ≥ n and k = n - (r-1)*r/2 - 1
 *            (0‑based column index)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"          /* provides seq_t calc_phi(seq_t) */

seq_t a(idx_t n)
{
    /* --- find the row r and column k corresponding to index n --- */
    idx_t r = 1;
    idx_t t = 1;                 /* cumulative count up to current row */
    while (t < n) {
        ++r;
        t += r;                  /* t = r*(r+1)/2 */
    }
    idx_t prev_t = t - r;        /* count before row r */
    idx_t k = n - prev_t - 1;    /* 0‑based column index */

    /* --- search for the smallest x with cototient(x) % r == k --- */
    seq_t target = (seq_t)k;
    for (seq_t x = 1; ; ++x) {
        seq_t phi = calc_phi(x);
        seq_t cototient = x - phi;
        if ((cototient % (seq_t)r) == target)
            return x;
    }
}
