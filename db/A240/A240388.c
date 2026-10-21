/*
 * A240388  A sequence related to the Stern sequence s(n) (A002487), defined
 *          by w(n) = s(3n)/2.
 * https://oeis.org/A240388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = A002487(3*n) / 2 (Stern's diatomic sequence at 3n,
 *            halved)
 * Range:     u8 n <= 3525, u16 n <= 8382, u32 n <= 8382, u64 n <= 8382,
 *            u128 n <= 8382, big any n
 * Helpers:   stern_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/stern_of.h"

/* Main entry for OEIS A240388: w(n) = s(3n)/2. */
seq_t a(idx_t n) {
    idx_t k = n * 3;          /* 3*n, safe for typical n values in tests  */
    seq_t val = stern_of(k);  /* compute s(3n) */
    return val / 2;           /* exact division by 2 (s(3n) is always even) */
}
