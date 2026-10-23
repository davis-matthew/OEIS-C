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
 * Helpers:   has_six_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_six_runs.h"

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
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
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
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
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
