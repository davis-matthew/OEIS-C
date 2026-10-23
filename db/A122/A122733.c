/*
 * A122733  Least sum of n positive cubes to have exactly n prime factors,
 *          with multiplicity.
 * https://oeis.org/A122733
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n for n ≥ 8; for 2 ≤ n < 8 computed by a brute‑force
 *            search
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   count_prime_factors, int_cube_root, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_prime_factors.h"
#include "../utils/int_cube_root.h"
#include "../utils/pow_uint.h"

/* Forward declarations of helpers */
static idx_t cube_sum_min(idx_t n);
static void brute_rec(idx_t depth, idx_t maxDepth, idx_t start,
                      idx_t maxSum, idx_t curSum, idx_t *best);

/* Main entry point for the sequence */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 */

    if (n >= 8)                           /* 2^n is minimal for n ≥ 8 */
        return pow_uint(2ULL, n);

    /* Cache results for n < 8 (indices 2–7) */
    static idx_t cache[8] = {0};          /* cache[0]..cache[1] unused */
    if (!cache[n])
        cache[n] = cube_sum_min(n);
    return (seq_t)cache[n];
}

/* ---------- Helper: brute‑force search for 2 ≤ n < 8 ---------- */
static idx_t cube_sum_min(idx_t n)
{
    const idx_t MAX_SUM = 2000;           /* safe upper bound for n < 8 */
    idx_t best = (idx_t)(-1);            /* largest possible value */

    brute_rec(0, n, 1, MAX_SUM, (idx_t)0, &best);
    return best;
}

/* Recursive generation of non‑decreasing sequences of cube roots.
   depth   – number of cubes chosen so far
   maxDepth– desired length n
   start   – smallest cube root allowed (enforces non‑decreasing)
   maxSum  – upper bound for the overall sum
   curSum  – sum of cubes chosen so far
   best    – pointer to current best sum found
*/
static void brute_rec(idx_t depth, idx_t maxDepth, idx_t start,
                      idx_t maxSum, idx_t curSum, idx_t *best)
{
    if (depth == maxDepth) {
        if (curSum < *best && count_prime_factors((seq_t)curSum) == (seq_t)maxDepth)
            *best = curSum;
        return;
    }

    idx_t remaining = maxSum - curSum;
    if (remaining < start * start * start)   /* no cube fits */
        return;

    idx_t maxRoot = int_cube_root(remaining);
    for (idx_t x = start; x <= maxRoot; ++x) {
        idx_t cube = x * x * x;
        if (curSum + cube > *best)           /* prune with current best */
            continue;
        brute_rec(depth + 1, maxDepth, x, maxSum, curSum + cube, best);
    }
}
