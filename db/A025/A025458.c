/*
 * A025458  Number of partitions of n into 5 positive cubes.
 * https://oeis.org/A025458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2943, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of partitions of n into exactly 5 positive cubes.
   A partition is an unordered multiset of 5 cubes whose sum is n. */

/* This implementation enumerates all non‑decreasing 5‑tuples of
   cube indices, using pruning to skip impossible branches.
   It works for any n ≥ 0 and returns 0 for n < 5. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* No partition is possible for n < 5. */
    if (n < 5) {
        return 0;
    }

    /* Determine the largest integer k with k^3 ≤ n. */
    idx_t maxk = 0;
    for (idx_t k = 1;; ++k) {
        idx_t c = k * k * k;
        if (c > n)
            break;
        maxk = k;
    }

    /* Pre‑compute all cubes up to maxk. */
    seq_t *cubes = (seq_t *)malloc(maxk * sizeof(seq_t));
    if (!cubes) {
        /* Allocation failure – return 0 as a safe fallback. */
        return 0;
    }
    for (idx_t i = 0; i < maxk; ++i) {
        idx_t j = i + 1;
        cubes[i] = (seq_t)j * (seq_t)j * (seq_t)j;
    }

    seq_t count = 0;

    /* Enumerate all non‑decreasing 5‑tuples of cube indices. */
    for (idx_t i1 = 0; i1 < maxk; ++i1) {
        seq_t c1 = cubes[i1];
        /* If even using the largest remaining cubes cannot reach n, skip. */
        if (c1 + 4 * cubes[maxk - 1] < n)
            continue;
        /* If 5 copies of the smallest cube already exceed n, no larger i1 is needed. */
        if (c1 * 5 > n)
            break;

        for (idx_t i2 = i1; i2 < maxk; ++i2) {
            seq_t s12 = c1 + cubes[i2];
            /* Need 3 remaining cubes; prune if impossible. */
            if (s12 + 3 * cubes[maxk - 1] < n)
                continue;

            for (idx_t i3 = i2; i3 < maxk; ++i3) {
                seq_t s123 = s12 + cubes[i3];
                /* Need 2 remaining cubes; prune if impossible. */
                if (s123 + 2 * cubes[maxk - 1] < n)
                    continue;

                for (idx_t i4 = i3; i4 < maxk; ++i4) {
                    seq_t s1234 = s123 + cubes[i4];
                    /* One remaining cube. If even the largest cube cannot complete the sum, prune. */
                    if (s1234 + cubes[maxk - 1] < n)
                        continue;

                    for (idx_t i5 = i4; i5 < maxk; ++i5) {
                        seq_t sum = s1234 + cubes[i5];
                        if (sum == n)
                            ++count;
                        else if (sum > n)  /* cubes are non‑decreasing */
                            break;
                    }
                }
            }
        }
    }

    free(cubes);
    return count;
}
