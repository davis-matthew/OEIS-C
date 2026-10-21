/*
 * A233521  Number of disjoint subsets s of 0..(n-1) such that, for every x in
 *          s, x^x (mod n) is in s.
 * https://oeis.org/A233521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of the digraph x → x^x mod n on
 *            {0,…,n-1}
 * Range:     u8 n <= 270, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* allocate arrays */
    idx_t *f = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    uint8_t *state = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!f || !state) { free(f); free(state); return 0; }

    /* compute f[x] = x^x mod n */
    for (idx_t i = 0; i < n; ++i) {
        seq_t v = mod_pow((seq_t)i, i, (seq_t)n);  /* 0^0 → 1 by definition */
        f[i] = (idx_t)v;
    }

    seq_t cycles = 0;
    /* traverse each component */
    for (idx_t i = 0; i < n; ++i) {
        if (state[i] == 0) {
            idx_t v = i;
            while (state[v] == 0) {          /* go forward until a visited node */
                state[v] = 1;
                v = f[v];
            }
            if (state[v] == 1)               /* found a new cycle */
                ++cycles;

            /* mark all nodes on this path as fully visited */
            v = i;
            while (state[v] == 1) {
                state[v] = 2;
                v = f[v];
            }
        }
    }

    free(f);
    free(state);
    return cycles;
}
