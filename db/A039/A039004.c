/*
 * A039004  Numbers whose base-4 representation has the same number of 1's and
 *          2's.
 * https://oeis.org/A039004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A039004: numbers whose base‑4 representation has
 * the same number of 1’s and 2’s.
 * The function enumerates non‑negative integers in increasing order
 * and returns the n‑th such number (1‑based index). */

#include <stdbool.h>
#include "../utils/fair.h"

/* a(n) – 1‑based index of the nth number with equal numbers of 1’s
 * and 2’s in its base‑4 representation. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 0;

    while (true)
    {
        if (fair(x))
        {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
