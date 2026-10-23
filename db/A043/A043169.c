/*
 * A043169  Numbers k such that 2 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   adj27or72
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043169: Numbers k such that 2 and 7 occur juxtaposed in the base‑8 representation
 * of k but not of k‑1. */
#include <stdbool.h>
#include "../utils/adj27or72.h"

/* Compute the nth element of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    while (count < n) {
        if (adj27or72(k) && !adj27or72(k - 1))
            ++count;
        ++k;
    }
    return k - 1;
}
