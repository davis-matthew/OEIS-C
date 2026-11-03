/*
 * A133603  The matrix-vector product A133566 * A000108.
 * https://oeis.org/A133603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n) for odd n; a(n) = C(n) + C(n-1) for even n, where
 *            C(k) is the Catalan number. a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 by definition */
        return (seq_t)1;
    }

    seq_t cat = catalan(n);          /* C(n) */

    if ((n & 1U) == 0) {             /* even n */
        cat += catalan(n - 1);       /* C(n-1) */
    }

    return cat;
}
