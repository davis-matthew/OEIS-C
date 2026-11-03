/*
 * A097513  Number of ways to label the vertices of the octahedron (or faces
 *          of the cube) with nonnegative integers summing to n, where
 *          labelings that differ only by rotation or reflection are
 *          considered the same.
 * https://oeis.org/A097513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of orbits of non‑negative integer 6‑tuples
 *            (x1,…,x6) with Σxi = n under the octahedral group of order 48
 *            (rotations and reflections of the cube). This is obtained by
 *            Burnside’s lemma: a(n) = (1/48) Σg f_g(n), where for a
 *            permutation g with cycle lengths l1,…,lk, f_g(n) is the
 *            number...
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types – supplied by the build system */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Generate the 48 symmetries of the cube acting on its 6 faces.
   Faces are numbered 0:+x, 1:-x, 2:+y, 3:-y, 4:+z, 5:-z. */
static void generate_octahedral_perms(unsigned char perms[48][6])
{
    static const int face_axis[6] = {0,0,1,1,2,2};
    static const int face_sign[6] = { 1,-1, 1,-1, 1,-1 };

    /* All 6 permutations of the axes. */
    static const int axis_perms[6][3] = {
        {0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}
    };

    int count = 0;
    for (int pi = 0; pi < 6; ++pi) {
        const int *p = axis_perms[pi];
        for (int s = 0; s < 8; ++s) {          /* 3 independent signs */
            int sign[3];
            sign[0] = (s & 1) ? 1 : -1;
            sign[1] = (s & 2) ? 1 : -1;
            sign[2] = (s & 4) ? 1 : -1;

            for (int f = 0; f < 6; ++f) {
                int a  = face_axis[f];
                int sgn = face_sign[f];
                int row = p[a];
                int val = sgn * sign[a];
                perms[count][f] = (unsigned char)(2 * row + (val < 0));
            }
            ++count;
        }
    }
    /* count should be 48 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Allocate memory for sums of fixed points up to n. */
    size_t N = (size_t)n + 1;
    seq_t *sum_f = mem_new(seq_t, N);   /* zeroed */
    if (!sum_f) return (seq_t)0;                       /* out of memory */

    /* Generate the group permutations once. */
    static unsigned char perms[48][6];
    static bool perms_done = false;
    if (!perms_done) {
        generate_octahedral_perms(perms);
        perms_done = true;
    }

    /* Temporary array for a single permutation's fixed‑point count. */
    seq_t *f = mem_new(seq_t, N);
    if (!f) { mem_free(sum_f); return (seq_t)0; }

    for (int g = 0; g < 48; ++g) {
        /* reset f */
        mem_set(f, 0, N * sizeof(seq_t));
        f[0] = 1;

        /* cycle decomposition of this permutation */
        bool visited[6] = {false};
        for (int i = 0; i < 6; ++i) {
            if (!visited[i]) {
                int len = 0;
                int j = i;
                do {
                    visited[j] = true;
                    j = perms[g][j];
                    ++len;
                } while (!visited[j]);

                /* DP contribution from a cycle of length len */
                for (size_t k = len; k < N; ++k) {
                    f[k] += f[k - len];
                }
            }
        }

        /* Accumulate this permutation's contribution. */
        for (size_t k = 0; k < N; ++k) {
            sum_f[k] += f[k];
        }
    }

    /* Final result: divide by |G| = 48. Integer division is exact. */
    seq_t result = sum_f[n] / (seq_t)48;

    mem_free(f);
    mem_free(sum_f);
    return result;
}
