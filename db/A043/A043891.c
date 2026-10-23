/*
 * A043891  Numbers k such that 1 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adjacent_13or31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search through integers k; return the n‑th k for which
   base‑5 representation of k contains “13” or “31” (adjacent 1‑3 or 3‑1),
   but the base‑5 representation of k + 1 does not. */
/* The sequence is A043891. */

#include <stdbool.h>
#include "../utils/has_adjacent_13or31.h"

seq_t a(idx_t n)
{
    seq_t k = 0;
    idx_t cnt = 0;

    while (1)
    {
        if (has_adjacent_13or31(k) && !has_adjacent_13or31(k + 1))
        {
            cnt++;
            if (cnt == n)
                return k;
        }
        k++;
    }
}
