/*
 * A007007  Valence of graph of maximal intersecting families of sets.
 * https://oeis.org/A007007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 0; for n >= 2, a(n) = binomial(n, floor((n-1)/2)) (=
 *            A037952(n))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   binomial, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }
    return binomial(n, (n - 1) / 2);
}
