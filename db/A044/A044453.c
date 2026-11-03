/*
 * A044453  Numbers k such that string 2,3 occurs in the base 4 representation
 *          of k but not of k+1.
 * https://oeis.org/A044453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2911, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9451 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044453
   Numbers k such that the string 2,3 occurs in the base‑4 representation of k
   but not of k+1.
   Implementation: sequential search with memoisation so calls to a(n)
   in any order are O(1) after first computation.
*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if base‑4 representation of x contains the substring "23". */
static bool contains_23_base4(seq_t x)
{
    /* Scan digits from least‑significant to most‑significant. */
    while (x != 0) {
        int cur = (int)(x % 4);
        x /= 4;
        if (cur == 3) {            /* potential start of "23" */
            if (x % 4 == 2) {
                return true;       /* found "23" */
            }
        }
    }
    return false;
}

/* Sequence a(n) for OEIS A044453.
   a(1) is the first occurring number satisfying the condition. */
seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order requests. */
    static seq_t *cache = NULL;      /* grows as needed */
    static idx_t cached_max = 0;     /* number of terms cached */
    static seq_t next_k = 0;         /* next k to check */

    /* Ensure cache has enough space. */
    if (cache == NULL) {
        cache = mem_new(seq_t, 16);
        if (cache == NULL) {
            abort(); /* memory failure */
        }
        cached_max = 0;
    }

    /* Grow cache if required. */
    while (cached_max < n) {
        /* Find next k with the property. */
        bool found = false;
        while (!found) {
            if (contains_23_base4(next_k) && !contains_23_base4(next_k + 1u)) {
                /* Append to cache. */
                if (cached_max == 0) {
                    cache[0] = next_k; /* first element */
                    cached_max = 1;
                } else {
                    /* Grow if needed. */
                    if (cached_max >= 8 && cached_max % 8 == 0) {
                        seq_t *new_cache = mem_resize(seq_t, cache, (cached_max * 2));
                        if (new_cache == NULL) {
                            abort();
                        }
                        cache = new_cache;
                    }
                    cache[cached_max++] = next_k;
                }
                found = true;
            }
            next_k++;
        }
    }

    return cache[n - 1];
}
