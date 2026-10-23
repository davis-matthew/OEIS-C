/*
 * A043697  Numbers whose base-8 representation has an even number of runs.
 * https://oeis.org/A043697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043697: a(n) is the nth positive integer whose base‑8
   representation has an even number of runs (blocks of identical
   consecutive digits). */
#include <stdbool.h>

/* Count runs in the base‑8 representation of x.  Return true when the
   number of runs is even. */
static bool has_even_runs(seq_t x) {
    int runs = 0;
    int last = -1;          /* sentinel for "no previous digit"       */
    seq_t y = x;            /* work on a copy to avoid modifying x    */
    while (y > 0) {
        int d = (int)(y % 8); /* current least‑significant octal digit */
        if (d != last) {
            runs++;
            last = d;
        }
        y /= 8;
    }
    return (runs % 2) == 0;
}

/* Return the n‑th member of the sequence (n ≥ 1). */
seq_t a(idx_t n) {
    idx_t cnt = 0;          /* how many qualifying numbers seen     */
    seq_t val = 8;          /* a(1) is 8, so start checking from here */
    while (1) {
        if (has_even_runs(val)) {
            cnt++;
            if (cnt == n)
                return val; /* found the desired term */
        }
        val++;
    }
}
