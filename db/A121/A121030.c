/*
 * A121030  Multiples of 10 containing a 10 in their decimal representation.
 * https://oeis.org/A121030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th multiple of 10 that contains
   the substring "10" in its decimal representation
   (by brute‑force search). */

#include <stdbool.h>
#include "../utils/contains10.h"

seq_t a(idx_t n)
{
    idx_t k = 0;              /* count of found terms */
    idx_t i = 1;              /* multiplier for 10 */

    while (k < n) {
        seq_t candidate = (seq_t)i * 10;
        if (contains10(candidate)) {
            ++k;
            if (k == n)
                return candidate;
        }
        ++i;
    }
    return 0; /* unreachable */
}
