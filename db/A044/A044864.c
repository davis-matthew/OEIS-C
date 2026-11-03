/*
 * A044864  Positive integers having the same number of base-8 runs of odd
 *          length as even.
 * https://oeis.org/A044864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A044864: Positive integers having the same number of
   base‑8 runs of odd length as runs of even length.
   This implementation searches for n‑th term by brute force, caching
   results to support out‑of‑order queries.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static bool equal_runs(seq_t x)
{
    if (x == 0) return false;          /* not a positive integer */
    unsigned odd = 0, even = 0;        /* counters for run parities */
    unsigned len = 0;                  /* current run length   */
    int prev = -1;                     /* previous digit, -1 means none yet */

    /* Process all octal digits of x, starting with the least significant */
    while (x > 0) {
        unsigned d = (unsigned)(x % 8);
        x /= 8;
        if (d == prev) {
            len++;                     /* extend current run */
        } else {                      /* new run starts */
            if (prev != -1) {          /* finalize previous run */
                if ((len & 1) == 0) even++; else odd++;
            }
            prev = d;
            len = 1;
        }
    }
    /* finalize the last run (if any) */
    if (prev != -1) {
        if ((len & 1) == 0) even++; else odd++;
    }
    return odd == even;
}

seq_t a(idx_t n)
{
    /* static cache for the sequence terms */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t have  = 0;      /* number of terms stored */
    static size_t cap   = 0;      /* allocation capacity */
    static seq_t next   = 1;      /* next candidate to test */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 64;
        cache = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n of them */
    while (have < n) {
        if (equal_runs(next)) {
            if (have == cap) {      /* double the capacity when needed */
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[have++] = next;   /* store the term */
        }
        ++next;                     /* advance to following candidate */
    }

    /* return the nth term (1‑based) */
    return cache[n - 1];
}
