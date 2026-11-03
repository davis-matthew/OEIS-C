/*
 * A043730  Numbers whose number of runs in their base-2 representation is
 *          congruent to 2 mod 5.
 * https://oeis.org/A043730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9861 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A043730: Numbers whose number of runs in their base‑2 representation is
 *  congruent to 2 mod 5.
 *  The implementation scans successive integers, counts binary runs, and
 *  keeps those with (runs % 5) == 2.  Results are memoised so that
 *  repeated calls are very fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* count_runs_in_binary: counts the number of runs in the binary
 * representation of x.  A run is a maximal substring of equal
 * consecutive bits.  For example, 11011100 has runs: 11 | 0 | 111 | 00
 * → 4 runs.  The function works for any unsigned integer type that
 * supports the usual bitwise operations. */
static inline unsigned int count_runs_in_binary(seq_t x)
{
    unsigned int runs = 0;
    bool prev_bit = false;
    bool first_bit = true;

    while (x != 0) {
        bool bit = ((x & 1) != 0);
        if (first_bit) {
            runs = 1;
            prev_bit = bit;
            first_bit = false;
        } else if (bit != prev_bit) {
            ++runs;
            prev_bit = bit;
        }
        x >>= 1;
    }
    return runs;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* a: returns the n‑th term of A043730 (n ≥ 1).  The sequence is memoised
 * in a static array so that subsequent calls are O(1) after the needed
 * terms have been generated. */
seq_t a(idx_t n)
{
    /* Static cache of generated terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t  size  = 0;     /* number of stored terms */
    static size_t  cap   = 0;     /* allocated capacity */
    static seq_t   next  = ((seq_t)2); /* next integer to test */

    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Grow the cache if necessary. */
        if (cap <= size) {
            size_t new_cap = cap ? cap * 2 : 4;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t x = next++;             /* candidate integer            */
        if (count_runs_in_binary(x) % 5 == 2) {
            cache[size++] = x;         /* store if it satisfies property */
        }
    }

    return cache[n - 1];
}
