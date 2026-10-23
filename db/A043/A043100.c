/*
 * A043100  Numbers k such that 0 and 1 occur juxtaposed in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A043100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adjacent_01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043100: numbers k such that 0 and 1 occur together in the base‑4
   representation of k but not of k‑1. The sequence is generated iteratively
   by testing each integer and counting the qualifying ones until the n‑th
   such number is found. */
#include <stdbool.h>
#include "../utils/has_adjacent_01.h"

/* Public sequence function.  It works for any seq_t that can be
   constructed from a uint64_t value (int, unsigned types or an
   arbitrary‑precision class with a suitable constructor). */
seq_t a(idx_t n)
{
    if (n == 0)                  /* OEIS offset is 1, but return 0 safely */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k = 1;

    while (1) {
        if (has_adjacent_01(k) && !has_adjacent_01(k - 1)) {
            count++;
            if (count == n)
                return (seq_t)k;   /* cast for arbitrary seq_t */
        }
        k++;
    }
}
