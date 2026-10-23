/*
 * A026601  Numbers k such that A026600(k) = 1.
 * https://oeis.org/A026601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A079498(n)+1 (positions of 1 in the fixed point of the
 *            morphism 1 → 123, 2 → 231, 3 → 312 )
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   symbol_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/symbol_at.h"

/* Return the n‑th term of A026601 (1-indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but guard against zero */
    idx_t count = 0;
    idx_t pos   = 0;
    while (1) {
        ++pos;
        if (symbol_at(pos) == (seq_t)1) {
            ++count;
            if (count == n)
                return (seq_t)pos;
        }
    }
}
