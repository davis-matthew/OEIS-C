/*
 * A044902  Base 16 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   hex_runs_alternate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration: the nth positive integer whose
   hexadecimal representation has run lengths alternating even,
   odd, even, …, starting with an even run. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/hex_runs_alternate.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;
    while (count < n) {
        candidate++;                     /* next integer */
        if (hex_runs_alternate(candidate)) {
            count++;
        }
    }
    return candidate;
}
