/*
 * A043883  Numbers k such that 1 and 2 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_adj_12or21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043883: Numbers k such that the base‑4 representation of k
   contains adjacent digits 1 and 2 (in either order) but the base‑4
   representation of k+1 does not.  Computed by brute‑force search. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adj_12or21.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k)
    {
        if (has_adj_12or21(k) && !has_adj_12or21(k + 1))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
