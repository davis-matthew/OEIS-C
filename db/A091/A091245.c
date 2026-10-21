/*
 * A091245  Number of reducible GF(2)[X]-polynomials in range [0,n].
 * https://oeis.org/A091245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of GF(2)[X] polynomials in the range [0,n] that
 *            are reducible (degree ≥ 2 and admit a non‑trivial factor).
 *            Implemented by a straightforward check of each integer up to n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_reducible, poly_deg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_reducible.h"
#include "../utils/poly_deg.h"

/*--- Main sequence function -----------------------------------------------------*/
seq_t a(idx_t n) {
    seq_t cumulative = 0;            /* start with zero count */
    for (idx_t i = 0; i <= n; ++i) {
        if (is_reducible(i)) cumulative++;
    }
    return cumulative;
}
