/*
 * A043676  Numbers whose base-16 representation has exactly 3 runs.
 * https://oeis.org/A043676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑16 representation has exactly three runs. */
/* Computed by scanning numbers and testing the run property. */

#include <stdbool.h>

/* Helper: true if x has exactly three runs in base 16. */
static inline bool has_three_runs(seq_t x)
{
    int runs = 0;
    int prev = -1;                 /* previous digit, -1 means none yet */
    while (x != 0) {
        int d = (int)(x & (seq_t)0xF);   /* least significant hex digit */
        if (d != prev) {
            runs++;
            if (runs > 3) return false;
            prev = d;
        }
        x >>= 4;                     /* move to next hex digit */
    }
    return runs == 3;
}

/* Return the nth such number (offset 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t v = (seq_t)257; ; ++v) {   /* 257 = 0x101, first candidate */
        if (has_three_runs(v)) {
            ++count;
            if (count == n) return v;
        }
    }
}
