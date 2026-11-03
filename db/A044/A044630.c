/*
 * A044630  Numbers n such that string 7,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑8 representation
 *            contains the substring "76" but whose successor n+1 does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches forward and caches found terms. */

#include <stdbool.h>   /* bool */
#include <stdlib.h>    /* realloc, NULL */
#include "../utils/mem.h"

static bool has76_base8(seq_t x)
{
    int prev = -1;                     /* previous (more significant) digit */
    while (x != 0) {
        int d = (int)(x % 8);          /* current less significant digit */
        if (prev == 6 && d == 7)       /* pattern 7 (more) followed by 6 (less) */
            return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset irrelevant */

    static seq_t *cache = NULL;           /* array of found terms */
    static size_t cap   = 0;              /* allocated capacity          */
    static size_t sz    = 0;              /* number of terms stored      */
    static seq_t cur    = 0;              /* next integer to test        */

    /* Initialise cursor if the cache is empty. */
    if (sz == 0) cur = 1;

    /* Generate terms until we have at least n of them. */
    while (sz < (size_t)n) {
        /* Allocate more space if needed. */
        if (sz == cap) {
            size_t newcap = (cap ? cap * 2 : 4);
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }

        seq_t next = cur + (seq_t)1;
        if (has76_base8(cur) && !has76_base8(next)) {
            cache[sz++] = cur;           /* store found term */
        }
        cur++;                           /* advance to next integer */
    }

    return cache[n - 1];
}
