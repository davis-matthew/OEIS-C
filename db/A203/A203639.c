/*
 * A203639  Multiplicative with a(p^e) = e*p^(e-1).
 * https://oeis.org/A203639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = multiplicative with a(p^e) = e*p^(e-1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9580 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n < 1, return 0 for safety */

    seq_t result = 1;
    idx_t remaining = n;
    idx_t e = 0;

    /* factor 2 */
    while ((remaining & 1) == 0) { /* remaining % 2 == 0 */
        e++;
        remaining >>= 1;           /* remaining /= 2 */
    }
    if (e > 0) {
        seq_t term = ipow_idx((seq_t)2, e - 1);
        term *= (seq_t)e;          /* term = e * 2^(e-1) */
        result *= term;
    }

    /* odd factors */
    for (idx_t p = 3; p <= remaining / p; p += 2) {
        if (remaining % p == 0) {
            e = 0;
            while (remaining % p == 0) {
                e++;
                remaining /= p;
            }
            seq_t term = ipow_idx((seq_t)p, e - 1);
            term *= (seq_t)e;      /* term = e * p^(e-1) */
            result *= term;
        }
    }

    /* remaining prime factor with exponent 1 gives multiplicative value 1, no change */
    return result;
}
