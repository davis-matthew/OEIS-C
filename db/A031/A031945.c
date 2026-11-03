/*
 * A031945  Numbers in which 0,1,2,3 all occur in base 4.
 * https://oeis.org/A031945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for base‑4 numbers that contain all
   four digits 0,1,2,3 at least once. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static int is_valid_base4(seq_t v)
{
    int mask = 0;
    while (v > 0) {
        int d = (int)(v % 4);
        mask |= 1 << d;
        v /= 4;
    }
    return mask == 0xF; /* bits for digits 0,1,2,3 set */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but guard anyway */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;
    static seq_t last_candidate = 0;

    /* grow cache when needed */
    if (len < n) {
        /* ensure enough capacity */
        while (len < n) {
            if (cap == 0) {
                cap = 128;
                cache = mem_new(seq_t, cap);
            } else if (len >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }

            /* find next valid term */
            do {
                last_candidate++;
            } while (!is_valid_base4(last_candidate));

            cache[len++] = last_candidate;
        }
    }
    return cache[n - 1];
}
