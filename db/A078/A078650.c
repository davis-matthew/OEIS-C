/*
 * A078650  2-A000002(n) where A000002 is the Kolakoski sequence.
 * https://oeis.org/A078650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 - K(n) where K is the Kolakoski sequence
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Ensure that we never work with an index <= 0, the offset is 1 */
    if (n == 0) return 0; /* undefined behavior for n < 1, but harmless */

    /* Static cache of the Kolakoski sequence (1‑based values 1 or 2) */
    static seq_t *kol = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static size_t len = 0;      /* number of computed elements */
    static idx_t  runIdx = 0;   /* index of the previous run length used */
    static seq_t  curr_val = 2; /* value to write next (2 after the initial 1) */
    static size_t run_cnt = 0;  /* count of how many of current run written */

    /* Allocate/expand cache if needed */
    ensure_cache(n, &kol, &cap);

    /* Initialisation on first call */
    if (len == 0) {
        kol[0] = 1;                 /* first Kolakoski term */
        len    = 1;                 /* one element already stored */
        runIdx = 0;                 /* previous run length is seq[0] */
        curr_val = 2;               /* next value to output */
        run_cnt = 0;                /* written 0 of current run */
    }

    /* Generate terms up to n (0‑based indices 0 .. n‑1) */
    while (len < n) {
        kol[len] = curr_val;     /* write the next term */
        ++run_cnt;               /* one more of current run written */

        /* After writing the run length determined by seq[runIdx + 1] */
        if (run_cnt == (size_t)kol[runIdx + 1]) {
            ++runIdx;             /* move to next run length index */
            curr_val = 3 - curr_val;   /* toggle 1 ↔ 2 */
            run_cnt = 0;          /* reset counter for the new run */
        }
        ++len;
    }

    /* a(n) = 2 - K(n), with K(n) = kol[n-1] (0‑based) */
    return 2 - kol[n - 1];
}
