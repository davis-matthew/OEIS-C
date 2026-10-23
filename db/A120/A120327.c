/*
 * A120327  Smallest nonsquarefree number >= n.
 * https://oeis.org/A120327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = smallest nonsquarefree number >= n by scanning upwards. */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        n = 1; /* Ensure the sequence starts at the OEIS offset. */
    }
    seq_t m = (seq_t)n;
    while (is_squarefree(m)) {
        m++;
    }
    return m;
}
