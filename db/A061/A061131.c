/*
 * A061131  Number of degree-n even permutations of order dividing 8.
 * https://oeis.org/A061131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Number of degree‑n even permutations of order dividing 8.
 *            Equivalent recurrence (DP over cycle structures with lengths
 *            1,2,4,8): Let e[n] = count of such permutations on n elements
 *            with even sign, let o[n] = same with odd sign. Then for n ≥ 1
 *            e[n] = Σ_{L∈{1,2,4,8}, L≤n} P(n−1, L−1) * e[n−L] + Σ_{...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

static seq_t *e = NULL;          /* even‑sign counts */
static seq_t *o = NULL;          /* odd‑sign counts */
static size_t cap = 0;           /* current capacity of arrays */
static idx_t maxComputed = (idx_t)-1;  /* -1 means uninitialised */

/* Ensure arrays can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (maxComputed != (idx_t)-1 && n <= maxComputed)
        return;                  /* already computed up to n */

    size_t newCap = cap ? cap : 16;
    while (newCap <= (size_t)n) newCap <<= 1;

    seq_t *newE = (seq_t *)realloc(e, newCap * sizeof(seq_t));
    seq_t *newO = (seq_t *)realloc(o, newCap * sizeof(seq_t));
    if (!newE || !newO) abort();

    /* Zero new slots */
    for (size_t i = cap; i < newCap; ++i) {
        newE[i] = 0;
        newO[i] = 0;
    }

    e = newE;
    o = newO;
    cap = newCap;
}

/* Principal function */
seq_t a(idx_t n)
{
    /* First time: initialise base case */
    if (maxComputed == (idx_t)-1) {
        ensure_capacity(n);
        e[0] = 1;
        o[0] = 0;
        maxComputed = 0;
    }

    /* Compute up to n if necessary */
    if (n > maxComputed) {
        ensure_capacity(n);
        /* Pre‑compute factorial terms for convenience (not strictly needed) */
        for (idx_t sz = maxComputed + 1; sz <= n; ++sz) {
            seq_t even = 0;
            seq_t odd  = 0;
            /* Consider all allowed cycle lengths */
            int lengths[4] = {1, 2, 4, 8};
            for (int idx = 0; idx < 4; ++idx) {
                int L = lengths[idx];
                if (sz < (idx_t)L) continue;

                /* Compute P(sz-1, L-1) = product_{t=0}^{L-2} (sz-1 - t) */
                seq_t perm = 1;
                for (int t = 0; t < (L - 1); ++t) {
                    perm *= (seq_t)(sz - 1 - t);
                }

                /* Determine which sign of previous state contributes */
                bool flip = ((L & 1) == 0);    /* L even => parity flips */

                if (!flip) {
                    /* L odd: parity unchanged */
                    even += perm * e[sz - L];
                    odd  += perm * o[sz - L];
                } else {
                    /* L even: parity flips */
                    even += perm * o[sz - L];
                    odd  += perm * e[sz - L];
                }
            }
            e[sz] = even;
            o[sz] = odd;
        }
        maxComputed = n;
    }

    return e[n];
}
