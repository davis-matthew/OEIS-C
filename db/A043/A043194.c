/*
 * A043194  Numbers k such that 1 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_1_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043194: numbers k such that the base‑9 representation of k contains
   the digits 1 and 8 as adjacent (in either order) but the base‑9 representation
   of k‑1 does not. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_adjacent_1_8.h"

/* Grow the static cache for the sequence.  Doubles the capacity or starts at
   16 if it is currently empty. */
static void grow_cache(idx_t cap, seq_t **valptr)
{
    idx_t newcap = (cap == 0) ? 16 : cap << 1;
    seq_t *newptr = (seq_t *)realloc(*valptr, newcap * sizeof(seq_t));
    *valptr = newptr;
}

/* Return the nth term of A043194.  The function uses static state to memoise
   previously computed terms; it is safe to call in any order. */
seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static idx_t vals_size = 0, vals_cap = 0;
    static idx_t next_k = 1;          /* next integer to test */
    static bool prev_has = false;     /* has_adjacent_1_8(k-1) */

    while (vals_size < n) {
        bool curr_has = has_adjacent_1_8(next_k);
        if (curr_has && !prev_has) {
            if (vals_size == vals_cap)
                grow_cache(vals_cap, &vals);
            vals[vals_size++] = (seq_t)next_k;
        }
        prev_has = curr_has;
        ++next_k;
    }
    return vals[n - 1];
}
