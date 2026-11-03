/*
 * A043678  Numbers whose base-16 representation has exactly 5 runs.
 * https://oeis.org/A043678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the n-th natural number whose base‑16 representation
   contains exactly five runs of equal hexadecimal digits.  */

/* Roughly a linear scan for increasing n; works well for n <= 10^4. */

#include <stdlib.h>
#include "../utils/mem.h"

static int runs_in_hex(seq_t v) {
    int runs = 0;
    int prev = -1;
    seq_t zero = 0;
    seq_t sixteen = (seq_t)16;
    while (v != zero) {
        seq_t d = v % sixteen;
        int di = (int)d;
        if (di != prev) {
            runs++;
            prev = di;
        }
        v /= sixteen;
    }
    return runs;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, a(1) is first value */

    /* Static state for on‑demand generation */
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t capacity = 0;
    static seq_t candidate = 1;
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        capacity = 64;
        seq = mem_new(seq_t, capacity);
    }

    /* Generate until we have at least n terms */
    while (size < n) {
        if (runs_in_hex(candidate) == 5) {
            if (size == capacity) {
                capacity *= 2;
                seq = mem_resize(seq_t, seq, capacity);
            }
            seq[size++] = candidate;
        }
        candidate++;
    }

    return seq[n - 1];
}
