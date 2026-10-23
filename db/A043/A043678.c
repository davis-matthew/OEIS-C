/*
 * A043678  Numbers whose base-16 representation has exactly 5 runs.
 * https://oeis.org/A043678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   runs_in_hex
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the n-th natural number whose base‑16 representation
   contains exactly five runs of equal hexadecimal digits.  */

/* Roughly a linear scan for increasing n; works well for n <= 10^4. */

#include <stdlib.h>
#include "../utils/runs_in_hex.h"

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
        seq = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }

    /* Generate until we have at least n terms */
    while (size < n) {
        if (runs_in_hex(candidate) == 5) {
            if (size == capacity) {
                capacity *= 2;
                seq = (seq_t *)realloc(seq, sizeof(seq_t) * capacity);
            }
            seq[size++] = candidate;
        }
        candidate++;
    }

    return seq[n - 1];
}
