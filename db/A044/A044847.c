/*
 * A044847  Positive integers having no fewer base-6 runs of even length than
 *          odd.
 * https://oeis.org/A044847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044847: Positive integers having no fewer base‑6 runs of even length than odd.
   The routine scans successive integers, counts runs of equal base‑6 digits,
   and stores the qualifying numbers in a static cache so that any order of
   calls to a(idx_t n) works efficiently.  Runs are counted once per
   number, giving O(#terms + √max_value) time, which is well below 2 s for
   n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term (1‑based) of sequence A044847.  */
seq_t a(idx_t n) {
    /* Static cache of found terms. */
    static seq_t *cache = NULL;   /* array of found terms   */
    static size_t cap   = 0;      /* allocated capacity    */
    static idx_t  count = 0;      /* how many terms are stored */
    static seq_t next_start = 1;  /* next integer to test   */

    if (n == 0) return 0;          /* undefined for index 0 */

    /* Grow the cache until it contains at least n terms. */
    while (count < n) {
        seq_t x = next_start;
        ++next_start;              /* prepare for next iteration */

        /* Skip trivial small numbers; the first qualifying term is 7. */
        if (x < 7)
            continue;

        /* Count runs of equal digits in base‑6. */
        const seq_t six = 6;
        seq_t d   = x % six;      /* least significant digit  */
        x   /= six;
        seq_t prev = d;
        uint64_t len = 1;          /* current run length       */
        uint64_t even = 0, odd = 0;

        while (x > 0) {
            d = x % six;
            if (d == prev)
                ++len;
            else {
                if ((len & 1) == 0) ++even; else ++odd;
                len = 1;
                prev = d;
            }
            x /= six;
        }
        /* handle last run */
        if ((len & 1) == 0) ++even; else ++odd;

        if (even >= odd) {        /* satisfies the property */
            if (count == (idx_t)cap) {   /* enlarge cache if needed */
                size_t newcap = (cap == 0 ? 8 : cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failed */
                cache = tmp;
                cap   = newcap;
            }
            cache[count++] = next_start - 1;
        }
    }
    return cache[n - 1];
}
