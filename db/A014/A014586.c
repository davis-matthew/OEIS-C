/*
 * A014586  Nim-Grundy function for Take-a-Square (or Subtract-a-Square) game.
 * https://oeis.org/A014586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Grundy values for the Subtract‑a‑Square game.
   Recurrence: a(0) = 0;
               a(n) = mex{ a(n - k²) : k ≥ 1, k² ≤ n }   (n ≥ 1) */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoisation table for Grundy values. */
    static seq_t *gr = NULL;
    static idx_t  gr_cap = 0;   /* allocated size of `gr`          */
    static idx_t  gr_len = 0;   /* number of entries already computed */

    /* Table for marking reachable Grundy numbers when searching mex. */
    static uint8_t *seen = NULL;
    static size_t   seen_cap = 0;

    /* Ensure storage big enough for indices up to `n`. */
    if (n >= gr_len) {
        idx_t new_cap = gr_cap ? gr_cap : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(gr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        gr      = tmp;
        gr_cap  = new_cap;
    }

    /* Compute Grundy values for uncovered indices up to `n`. */
    for (idx_t i = gr_len; i <= n; ++i) {
        if (i == 0) {          /* base case */
            gr[0] = 0;
            continue;
        }

        /* Ensure `seen` can cover possible Grundy values. */
        if (seen_cap <= i + 1) {
            size_t new_cap = seen_cap ? seen_cap : 8;
            while (new_cap <= i + 1) new_cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(seen, new_cap);
            if (!tmp) exit(1);
            seen     = tmp;
            seen_cap = new_cap;
        }
        /* Clear the `seen` bitmap for this position. */
        memset(seen, 0, i + 2);

        /* Compute floor(sqrt(i)) without floating point. */
        idx_t lo = 0, hi = i, limit = 0;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            idx_t sq  = mid * mid;
            if (sq <= i) { limit = mid; lo = mid + 1; }
            else          { hi = mid - 1; }
        }

        /* Mark Grundy numbers reachable by subtracting a square. */
        for (idx_t k = 1; k <= limit; ++k) {
            idx_t sub = i - k * k;
            seq_t g   = gr[sub];
            size_t idx = (size_t)g;
            if (idx < seen_cap) seen[idx] = 1;
        }

        /* Mex: first non‑marked number. */
        seq_t mex = 0;
        while ((size_t)mex < seen_cap && seen[(size_t)mex]) ++mex;
        gr[i] = mex;
    }

    /* Update the highest computed index. */
    if (gr_len < n + 1) gr_len = n + 1;

    return gr[n];
}
