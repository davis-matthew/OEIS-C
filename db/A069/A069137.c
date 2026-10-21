/*
 * A069137  Numbers which are sums of neither 1, 2, 3, 4, 5 or 6 nonnegative
 *          cubes.
 * https://oeis.org/A069137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 138, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: computes numbers that cannot be represented as a sum of at most
   six non‑negative cubes.  The function expands a sieve until the
   requested term is found. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/int_cube_root.h"

/* Shift-or helper:  out |= in << shift.
   The bitset has 'words' 64‑bit words and represents integers
   0..B, where B < words*64.  The shift is in bits. */
static inline void shift_or(const uint64_t *in, uint64_t *out,
                            size_t shift, size_t words)
{
    size_t wshift = shift >> 6;          /* whole‑word shift */
    uint8_t  bits   = shift & 63;        /* intra‑word shift */

    for (size_t i = 0; i < words; ++i) {
        uint64_t val = in[i];
        if (!val) continue;
        size_t j = i + wshift;
        if (j >= words) break;          /* shift would exceed limit */
        if (bits == 0) {
            out[j] |= val;
        } else {
            out[j] |= val << bits;
            if (j + 1 < words)
                out[j + 1] |= val >> (64 - bits);
        }
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  Initialise a low bound that surely
       contains many terms, then double it until the n‑th term is found. */
    idx_t B = 200;                 /* initial sieve limit */
    while (1) {
        size_t words = ((size_t)B >> 6) + 1;          /* bits 0..B inclusive */

        /* Allocate and initialise bitsets.  0 is always reachable. */
        uint64_t *prev = (uint64_t *)calloc(words, sizeof(uint64_t));
        uint64_t *curr = (uint64_t *)calloc(words, sizeof(uint64_t));
        prev[0] = 1ULL;                     /* 0 reachable with 0 cubes */

        /* Build list of cubes up to B. */
        idx_t max_c = int_cube_root(B);
        idx_t *cubes = (idx_t *)malloc(sizeof(idx_t) * (size_t)max_c);
        for (idx_t i = 1; i <= max_c; ++i)
            cubes[i - 1] = i * i * i;         /* i^3 fits in idx_t for B << 2^32 */

        /* Perform convolution for 1..6 cubes. */
        for (int k = 1; k <= 6; ++k) {
            memcpy(curr, prev, words * sizeof(uint64_t));   /* keep earlier reps */
            for (idx_t ci = 0; ci < max_c; ++ci)
                shift_or(prev, curr, (size_t)cubes[ci], words);
            /* swap arrays */
            uint64_t *tmp = prev; prev = curr; curr = tmp;
        }

        /* Count missing numbers up to B. */
        idx_t missing = 0;
        for (idx_t i = 1; i <= B; ++i) {
            size_t w = (size_t)i >> 6;
            uint64_t mask = 1ULL << ((size_t)i & 63);
            if ((prev[w] & mask) == 0) {
                ++missing;
                if (missing == n) {
                    /* Clean up before returning. */
                    free(prev);
                    free(curr);
                    free(cubes);
                    return (seq_t)i;
                }
            }
        }

        /* Not enough numbers found; enlarge sieve and try again. */
        free(prev);
        free(curr);
        free(cubes);
        B <<= 1;  /* double B */
    }
}
