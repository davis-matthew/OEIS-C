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
 * Helpers:   generate_octahedral_perms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types – supplied by the build system */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/generate_octahedral_perms.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Allocate memory for sums of fixed points up to n. */
    size_t N = (size_t)n + 1;
    seq_t *sum_f = (seq_t *)calloc(N, sizeof(seq_t));   /* zeroed */
    if (!sum_f) return (seq_t)0;                       /* out of memory */

    /* Generate the group permutations once. */
    static unsigned char perms[48][6];
    static bool perms_done = false;
    if (!perms_done) {
        generate_octahedral_perms(perms);
        perms_done = true;
    }

    /* Temporary array for a single permutation's fixed‑point count. */
    seq_t *f = (seq_t *)calloc(N, sizeof(seq_t));
    if (!f) { free(sum_f); return (seq_t)0; }

    for (int g = 0; g < 48; ++g) {
        /* reset f */
        memset(f, 0, N * sizeof(seq_t));
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

    free(f);
    free(sum_f);
    return result;
}
