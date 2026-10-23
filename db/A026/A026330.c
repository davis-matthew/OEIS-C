/*
 * A026330  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| <= 1 for i = 1,2,...,n,
 *          s(0) = 2, s(2n) = n+1. Also a(n) = T(2n,n+1), where T is the array
 *          in A026323.
 * https://oeis.org/A026330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026330: number of walks of length 2 n with steps −1, 0, +1
   starting at height 2 and ending at height n+1 while never going below 0.
   Computation is performed by a simple dynamic‑programming algorithm. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* total number of steps equals 2 n, maximum possible height is 2 + 2 n,
       so we need an array of length 2 n + 3 to store indices up to 2 + 2 n. */
    size_t steps = 2 * n;
    size_t size   = steps + 3;          /* indices 0 … 2+2n */

#ifdef __cplusplus
    seq_t *cur = new seq_t[size];
    seq_t *nxt = new seq_t[size];
#else
    seq_t *cur = malloc(size * sizeof(seq_t));
    seq_t *nxt = malloc(size * sizeof(seq_t));
#endif
    if (!cur || !nxt) {
        /* allocation failed – behave gracefully */
        return 0;
    }

    /* initialise to zero */
    for (size_t i = 0; i < size; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }
    cur[2] = 1;          /* start at height 2 */

    for (size_t step = 1; step <= steps; ++step) {
        size_t maxBefore = 2 + (step - 1);
        size_t maxAfter  = 2 + step;

        /* clear next row up to the height that can be reached after this step */
        for (size_t i = 0; i <= maxAfter; ++i) nxt[i] = 0;

        for (size_t h = 0; h <= maxBefore; ++h) {
            seq_t val = cur[h];
            if (val == 0) continue;

            /* step up */
            nxt[h + 1] += val;
            /* step horizontally */
            nxt[h] += val;
            /* step down, staying nonnegative */
            if (h > 0) nxt[h - 1] += val;
        }

        /* swap rows for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = cur[n + 1];   /* ending height is n+1 */

#ifdef __cplusplus
    delete[] cur;
    delete[] nxt;
#else
    free(cur);
    free(nxt);
#endif

    return result;
}
