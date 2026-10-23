/*
 * A181323  Number of ways of placing n rods of length 3 in a 3x3x3 cubic
 *          lattice without any two rods intersecting (0 <= n <= 9).
 * https://oeis.org/A181323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A181323 (Number of ways of placing
   n rods of length 3 in a 3×3×3 cubic lattice without any two rods intersecting).
   It enumerates all independent sets of rods (each rod occupies exactly three
   unit cells along a straight axis) by recursion.  The number of rods is at most
   27, and any solution contains at most 9 disjoint rods, so the recursion
   explores at most a few million states, easily within time limits. */

#include <stdint.h>
#include <stdbool.h>

#define NUM_RODS 27

static uint32_t rod_masks[NUM_RODS];

static seq_t counts[10];
static bool initialized = false;

/* Convert 3D coordinates into a single bit position (0..26). */
static inline uint32_t pos(uint32_t x, uint32_t y, uint32_t z) {
    return (z * 9u) + (y * 3u) + x;
}

/* Recursive enumeration of all non‑intersecting subsets of rods. */
static void dfs(int start, int sz, uint32_t cells) {
    counts[sz] += 1;
    for (int i = start; i < NUM_RODS; ++i) {
        uint32_t m = rod_masks[i];
        if (cells & m) continue;            /* intersecting rod */
        dfs(i + 1, sz + 1, cells | m);
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        /* Build masks for the 27 rods: first x‑rods, then y‑rods, then z‑rods. */
        int r = 0;
        /* x‑rods: fixed y,z; varying x. */
        for (uint32_t y = 0; y < 3; ++y) {
            for (uint32_t z = 0; z < 3; ++z) {
                uint32_t m = 0;
                for (uint32_t x = 0; x < 3; ++x) m |= 1u << pos(x, y, z);
                rod_masks[r++] = m;
            }
        }
        /* y‑rods: fixed x,z; varying y. */
        for (uint32_t x = 0; x < 3; ++x) {
            for (uint32_t z = 0; z < 3; ++z) {
                uint32_t m = 0;
                for (uint32_t y = 0; y < 3; ++y) m |= 1u << pos(x, y, z);
                rod_masks[r++] = m;
            }
        }
        /* z‑rods: fixed x,y; varying z. */
        for (uint32_t x = 0; x < 3; ++x) {
            for (uint32_t y = 0; y < 3; ++y) {
                uint32_t m = 0;
                for (uint32_t z = 0; z < 3; ++z) m |= 1u << pos(x, y, z);
                rod_masks[r++] = m;
            }
        }
        /* Enumerate all independent sets of rods. */
        for (int i = 0; i < 10; ++i) counts[i] = 0;
        dfs(0, 0, 0);
        initialized = true;
    }
    if (n < 10)
        return counts[(int)n];
    else
        return 0;
}
