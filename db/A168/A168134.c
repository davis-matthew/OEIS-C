/*
 * A168134  Numbers not of the form 7*k+11*m (with nonnegative k, m).
 * https://oeis.org/A168134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   _reprable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers NOT of the form 7*k + 11*m with non‑negative k, m.
   The function scans integers from 1 upward, counts those that are not
   representable as a non‑negative linear combination of 7 and 11,
   and returns the n‑th such integer. Since 7 and 11 are coprime,
   the largest non‑representable integer is 59, so the sequence ends
   after 30 terms. For n larger than that the function returns 0. */
#include <stdbool.h>
#include "../utils/_reprable.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n = 0, but guard anyway */

    idx_t count = 0;
    for (seq_t x = 1; x <= 59; ++x) {
        if (!_reprable(x)) {
            ++count;
            if (count == n) return x;
        }
    }
    return 0;   /* n is larger than the number of non‑representable integers ≤59 */
}
