/*
 * A090861  Permutation of natural numbers arising from a spiral.
 * https://oeis.org/A090861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cc_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A090861 (counter‑clockwise spiral index → clockwise
   spiral value).  The function generates the counter‑clockwise
   coordinates for the nth step and then evaluates the clockwise
   spiral value at that coordinate.  */
#include <stdint.h>
#include <inttypes.h>
#include "../utils/cc_coords.h"

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    int64_t x, y;
    cc_coords(n, &x, &y);

    /* layer of the clockwise spiral */
    int64_t ax = (x >= 0) ? x : -x;
    int64_t ay = (y >= 0) ? y : -y;
    idx_t k = (ax > ay) ? (idx_t)ax : (idx_t)ay;
    if (k == 0) {          /* only possible for n==1 */
        return (seq_t)1;
    }

    /* corner value for layer k: (2k+1)^2 using seq_t arithmetic */
    seq_t side = ((seq_t)k) * 2 + 1;
    seq_t max_num = side * side;

    /* number of steps from the bottom‑right corner ((k,-k)) to (x,y) */
    long long step;
    if (y == -(long long)k) {                /* bottom side */
        step = k - x;
    } else if (x == -(long long)k) {          /* left side */
        step = 2 * k + (y + k);
    } else if (y == (long long)k) {           /* top side */
        step = 4 * k + (x + k);
    } else {                                  /* right side (x == k) */
        step = 6 * k + (k - y);
    }

    /* final value */
    return max_num - (seq_t)step;
}
