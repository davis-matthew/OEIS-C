/*
 * A043606  Numbers whose base-5 representation has exactly 6 runs.
 * https://oeis.org/A043606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains exactly 6 runs of identical consecutive digits
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has_six_runs(idx_t x)
{
    if (x == 0) return false;          /* only used for x>0 in this routine */
    idx_t prev = 5;                    /* so that the first digit always counts */
    int runs = 0;
    while (x > 0) {
        int d = (int)(x % 5);
        if (d != prev) {
            runs++;
            prev = d;
            if (runs > 6) return false; /* early exit */
        }
        x /= 5;
    }
    return runs == 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, return 0 */

    /* Static cache of terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t next = 1;             /* next integer to test */

    /* Grow cache when needed */
    if (sz < n) {
        if (cap == 0) {
            cap = 1024;
            cache = mem_new(seq_t, cap);
        }
        if (cache == NULL)
            return 0;                  /* out of memory */

        while (sz < n) {
            /* search for next number with exactly 6 runs */
            while (!has_six_runs(next))
                next++;

            /* append to cache */
            if (sz == cap) {
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL)
                    return 0;          /* out of memory */
                cache = tmp;
            }
            cache[sz++] = (seq_t)next;
            next++;
        }
    }

    return cache[n - 1];
}
