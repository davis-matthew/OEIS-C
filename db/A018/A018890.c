/*
 * A018890  Numbers whose smallest expression as a sum of positive cubes
 *          requires exactly 7 cubes.
 * https://oeis.org/A018890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the n-th number whose minimal sum of positive cubes has
 *            length exactly 7 (computed by dynamic programming)
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   ensure_cap_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine uses only integer arithmetic and works for all
   integer types supplied by the build (seq_t and idx_t). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cap_idx.h"

/* ----- Private state ----- */
static uint8_t *minCubes = NULL;        /* minimal cubes for each value (1 byte) */
static size_t  minCap   = 0;
static idx_t   maxChecked = 0;          /* largest value processed */

static idx_t *cubeVals   = NULL;        /* cube values i^3 */
static size_t  cubeCap   = 0;
static size_t  cubeLen   = 0;
static idx_t   cubeRoot  = 0;           /* largest root added */

static idx_t *terms = NULL;             /* terms with minimal length 7 */
static size_t  termCap  = 0;
static size_t  termLen  = 0;

/* ----- Helper: ensure array capacity ----- */
static inline void ensure_cap_uint8(uint8_t **ptr, size_t *cap, size_t newCap)
{
    if (*cap < newCap) {
        size_t ncap = *cap ? *cap * 2 : 16;
        while (ncap < newCap) ncap <<= 1;
        *ptr = (uint8_t *)realloc(*ptr, ncap * sizeof(uint8_t));
        *cap = ncap;
    }
}

/* ----- Add new cubes up to 'limit' ----- */
static void add_cubes_upto(idx_t limit)
{
    while (1) {
        idx_t r   = cubeRoot + 1;
        idx_t cub = r * r * r;
        if (cub > limit) break;
        cubeRoot = r;
        ensure_cap_idx(&cubeVals, &cubeCap, cubeLen + 1);
        cubeVals[cubeLen++] = cub;
    }
}

/* ----- Extend minCubes up to 'newMax' and collect term indices ----- */
static void extend_to(idx_t newMax)
{
    if (newMax <= maxChecked) return;

    /* Guarantee minCubes array size */
    ensure_cap_uint8(&minCubes, &minCap, (size_t)(newMax + 1));
    if (maxChecked == 0) minCubes[0] = 0;  /* base case */

    for (idx_t v = maxChecked + 1; v <= newMax; ++v) {
        add_cubes_upto(v);

        uint8_t best = 255;                      /* large sentinel */
        for (size_t i = 0; i < cubeLen; ++i) {
            idx_t c = cubeVals[i];
            if (c > v) break;
            uint8_t cand = minCubes[v - c] + 1;
            if (cand < best) best = cand;
        }
        minCubes[v] = best;

        if (best == 7) {
            ensure_cap_idx(&terms, &termCap, termLen + 1);
            terms[termLen++] = v;
        }
    }
    maxChecked = newMax;
}

/* ----- Ensure that at least 'need' terms have been found ----- */
static void ensure_n_terms(size_t need)
{
    while (termLen < need) {
        /* Increase the search window; 10000 is ample for the known data. */
        extend_to(maxChecked + 10000);
    }
}

/* ----- Public routine ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for index 0 */

    size_t nn = (size_t)n;
    ensure_n_terms(nn);
    return (seq_t)terms[nn - 1];
}
