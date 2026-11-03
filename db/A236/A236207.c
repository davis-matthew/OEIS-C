/*
 * A236207  Numbers not divisible by 5 or 11.
 * https://oeis.org/A236207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers not divisible by 5 or 11
    a(n) = the nth positive integer that is not a multiple of 5 or 11.
    This implementation uses a small cache so that repeated calls are fast
    and works for any n ≥ 1.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t count = 0;    /* number of cached terms */
    static idx_t next  = 1;    /* next integer to test for inclusion */
    static bool   done  = false;

    /* Ensure cache contains at least n terms */
    if (!done) { done = true; }
    if (count >= n) goto RETURN;

    if (cap < (size_t)n)            /* grow cache if necessary */
        grow_seq(&cache, &cap, (size_t)n);

    while (count < n) {
        if (next % 5U != 0 && next % 11U != 0) {
            cache[count] = (seq_t)next;
            count++;
        }
        next++;
    }

RETURN:
    return cache[n - 1];
}
