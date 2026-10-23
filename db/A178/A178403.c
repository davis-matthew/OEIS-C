/*
 * A178403  Numbers containing the rounded up arithmetic mean of their digits
 *          at least once, cf. A004427.
 * https://oeis.org/A178403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_mean_ceil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers containing the rounded up arithmetic mean of their digits
   at least once (OEIS A178403).  A(n) is the n‑th such integer
   (a(1) = 0). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_mean_ceil.h"

/* Generates the sequence a(n) = A178403. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = 0;

    while (true) {
        if (contains_mean_ceil(num)) {
            found++;
            if (found == n) return num;
        }
        num += 1;
    }
}
