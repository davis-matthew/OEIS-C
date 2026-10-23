/*
 * A016018  Least k such that (tau(k^3)+2)/3=n.
 * https://oeis.org/A016018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (tau(k^3)+2)/3 = n In terms of the
 *            index used by OEIS (offset 0), this is equivalent to all k such
 *            that tau(k^3) = 3*n + 1. The function uses a depth‑first search
 *            over factorizations of 3*n+1 into factors ≡ 1 (mod 3) and
 *            assigns the resulting exponents to the smallest pri...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* First 16 primes – enough for any n up to about 10000. */
static const uint32_t PRIMES[16] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53
};

#define MAX_TARG 30001          /* 3*10000 + 1 */

static seq_t  CACHE[MAX_TARG + 1];
static bool   DONE  [MAX_TARG + 1] = { false };

/* Integer power – works for any seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* Recursive search for all multisets of exponents producing   */
static void dfs(uint64_t target, int depth, uint64_t last_factor,
                uint64_t *exps, bool *has_best, seq_t *best)
{
    if (target == 1) {
        /* All factors chosen, build candidate k. */
        seq_t k = (seq_t)1;
        for (int i = 0; i < depth; ++i) {
            k *= pow_seq((seq_t)PRIMES[i], (idx_t)exps[i]);
        }
        if (!*has_best || k < *best) {
            *has_best = true;
            *best     = k;
        }
        return;
    }

    /* Enumerate factors d of target such that d ≡ 1 (mod 3) and d ≤ last_factor. */
    for (uint64_t d = 4; d <= target && d <= last_factor; ++d) {
        if (target % d != 0) continue;
        if (d % 3 != 1)      continue;
        exps[depth] = (d - 1) / 3;
        dfs(target / d, depth + 1, d, exps, has_best, best);
    }
}

/* Compute minimal k for a given target T = 3*n + 1. */
static seq_t minimal_for_target(uint64_t target)
{
    /* Cached results for small targets. */
    if (target <= MAX_TARG && DONE[target]) {
        return CACHE[target];
    }

    uint64_t exps[16];
    bool     has_best = false;
    seq_t    best = 0;

    dfs(target, 0, target, exps, &has_best, &best);

    if (target <= MAX_TARG) {
        DONE[target] = true;
        CACHE[target] = best;
    }
    return best;
}

seq_t a(idx_t n)
{
    /* For offset 0, the required target is 3*n + 1. */
    uint64_t target = (uint64_t)3 * n + 1u;
    return minimal_for_target(target);
}
