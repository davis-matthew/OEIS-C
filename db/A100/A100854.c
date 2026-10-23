/*
 * A100854  Least number of positive cubes that sum to n^2.
 * https://oeis.org/A100854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least number of positive cubes that sum to n² (OEIS
 *            A100854)
 * Range:     u8 n <= 14, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static storage for cubes */
static seq_t *cubes = NULL;
static size_t cube_count   = 0;
static size_t cube_capacity = 0;
static idx_t  last_cube_root = 0;
static seq_t  max_cube_value = 0;

/* grow the cubes array so that it contains all cubes ≤ target */
static void ensure_cubes(seq_t target)
{
    if (target <= max_cube_value) return;

    if (cube_capacity == 0) {
        cube_capacity = 64;
        cubes = (seq_t *)malloc(cube_capacity * sizeof(seq_t));
    }

    while (1) {
        idx_t root = last_cube_root + 1;
        seq_t cube = (seq_t)root * (seq_t)root * (seq_t)root;
        if (cube > target) break;

        if (cube_count >= cube_capacity) {
            cube_capacity *= 2;
            cubes = (seq_t *)realloc(cubes, cube_capacity * sizeof(seq_t));
        }
        cubes[cube_count++] = cube;
        last_cube_root = root;
        max_cube_value = cube;
    }
}

/* depth‑bounded DFS: can we reach rem using at most depth cubes,
   with the largest usable cube index ≤ maxIdx ? */
static bool dfs(seq_t rem, idx_t maxIdx, unsigned depth)
{
    if (rem == 0) return true;
    if (depth == 0) return false;
    if (maxIdx == (idx_t)(-1)) return false;

    seq_t cube = cubes[maxIdx];

    /* simple bounds that rule out impossibility */
    if (rem < (seq_t)depth) return false;
    if (rem > cube * (seq_t)depth) return false;

    /* check whether we can finish with only this cube */
    if (rem % cube == 0) {
        seq_t q = rem / cube;
        if (q <= depth) return true;
    }

    /* try using k copies of this cube (from 1 up to the limit) */
    seq_t max_use_t = rem / cube;
    unsigned max_use = (unsigned)(max_use_t > depth ? depth : max_use_t);
    for (unsigned k = max_use; k > 0; --k) {
        seq_t new_rem = rem - cube * (seq_t)k;
        if (dfs(new_rem, maxIdx - 1, depth - k))
            return true;
    }

    /* try not using this cube at all */
    return dfs(rem, maxIdx - 1, depth);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined but harmless */

    seq_t target = (seq_t)n * (seq_t)n;
    ensure_cubes(target);

    idx_t maxIdx = cube_count - 1;

    /* search for the minimum depth (conservative upper bound 12) */
    for (unsigned depth = 1; depth <= 12; ++depth) {
        if (dfs(target, maxIdx, depth))
            return (seq_t)depth;
    }

    /* should never occur; fall back */
    return (seq_t)13;
}
