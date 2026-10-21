/*
 * A051343  Number of ways of writing n as a sum of 3 nonnegative cubes
 *          (counted naively).
 * https://oeis.org/A051343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered triples (i,j,k) of non‑negative
 *            integers * such that i^3 + j^3 + k^3 = n (counted naively)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* The build supplies seq_t (an unsigned integer type or an
 * arbitrary‑precision integer class) and idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *memo   = NULL;           /* a(0)..a(capacity-1) */
    static size_t capacity = 0;
    static uint64_t *cubes = NULL;         /* list of i^3 values */
    static size_t cubesSize = 0;            /* number of cubes stored */

    /* Ensure we have entries up to n and rebuild cube table if needed */
    if (n + 1 > capacity) {
        size_t newCap = (size_t)(n + 1);

        /* allocate new memo array */
        seq_t *newMemo = (seq_t*)malloc(newCap * sizeof(seq_t));
        if (!newMemo) exit(1);
        if (memo) {
            memcpy(newMemo, memo, capacity * sizeof(seq_t));
            free(memo);
        }
        memset(newMemo + capacity, 0, (newCap - capacity) * sizeof(seq_t));
        memo = newMemo;
        capacity = newCap;

        /* recompute cube table up to the new maximum */
        idx_t maxRoot = 0;
        while ((uint64_t)(maxRoot + 1) * (maxRoot + 1) * (maxRoot + 1) <= n)
            ++maxRoot;
        size_t newCubesSize = (size_t)maxRoot + 1;
        uint64_t *newCubes = (uint64_t*)malloc(newCubesSize * sizeof(uint64_t));
        if (!newCubes) exit(1);
        for (size_t i = 0; i < newCubesSize; ++i)
            newCubes[i] = (uint64_t)i * i * i;
        free(cubes);
        cubes     = newCubes;
        cubesSize = newCubesSize;
    }

    /* compute missing entries */
    for (size_t idx = capacity; idx < capacity; ++idx) {
        /* inside loop – not reached as capacity is already up to n+1 */
    }

    /* compute a(i) for i from previous capacity to (n) */
    for (size_t i = capacity - (capacity > 0 ? 1 : 0); i <= (size_t)n; ++i) {
        seq_t count = 0;
        /* number of cube roots */
        size_t maxRoot = cubesSize - 1;
        idx_t target = (idx_t)i;

        for (size_t k = 0; k <= maxRoot; ++k) {
            uint64_t ck = cubes[k];
            if (ck > target) break;
            idx_t rem = target - ck;
            for (size_t a = 0; a <= maxRoot; ++a) {
                uint64_t ca = cubes[a];
                if (ca > rem) break;
                for (size_t b = 0; b <= maxRoot; ++b) {
                    uint64_t cb = cubes[b];
                    uint64_t sum = ca + cb;
                    if (sum == rem)
                        ++count;
                    else if (sum > rem)
                        break;
                }
            }
        }
        memo[i] = count;
    }

    return memo[n];
}
