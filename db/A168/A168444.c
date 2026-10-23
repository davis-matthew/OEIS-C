/*
 * A168444  Number of partitions of the set {1,2,...,n} such that no block is
 *          a sequence of consecutive integers (including 1-element blocks).
 * https://oeis.org/A168444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = b(n) – b(n‑1) where b(0) = 1, b(n) = Σ_{k=⌈n/2⌉}^{n}
 *            binom(k,n‑k)·(-1)^{n‑k}·B(k), B(0) = 1, B(n) = Σ_{k=0}^{n‑1}
 *            binom(n‑1,k)·B(k) (Bell numbers) This implementation caches Bell
 *            numbers and the b(n) values for all n requested so that later
 *            calls are O(1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* cached Bell numbers B[0..] */
static seq_t *Bell      = NULL;
/* cached b values b[0..] */
static seq_t *bVals     = NULL;
/* current allocated size of Bell and bVals, in elements */
static size_t BellCap  = 0;

/* Ensure Bell[0..n] and bVals[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if the cache already covers n. */
    if (BellCap > 0 && (size_t)n < BellCap)
        return;

    /* Allocate the new capacity, at least n + 1 entries. */
    size_t newCap = (size_t)n + 1;
    Bell  = (seq_t *)realloc(Bell,  newCap * sizeof(seq_t));
    bVals = (seq_t *)realloc(bVals, newCap * sizeof(seq_t));
    if (!Bell || !bVals)
        abort();          /* out of memory */

    /* First allocation: initialise B[0] and b[0] */
    if (BellCap == 0) {
        Bell[0]  = 1;      /* B(0) = 1 */
        bVals[0] = 1;      /* b(0) = 1 by definition */
    }

    /* Compute Bell numbers for new indices (starting after the previous cap). */
    idx_t startBell = (BellCap == 0) ? 1 : (idx_t)BellCap;
    for (idx_t i = startBell; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom(i - 1, k) * Bell[k];
            sum += term;
        }
        Bell[i] = sum;
    }

    /* Compute b(n) for new indices. */
    idx_t startIdx = (BellCap == 0) ? 1 : (idx_t)BellCap;
    for (idx_t i = startIdx; i <= n; ++i) {
        idx_t k_start = (i + 1) >> 1;      /* ceil(i/2) */
        seq_t pos = 0, neg = 0;
        for (idx_t k = k_start; k <= i; ++k) {
            idx_t d = i - k;               /* n‑k */
            seq_t term = Bell[k] * binom(k, d);
            if ((d & 1u) == 0)
                pos += term;
            else
                neg += term;
        }
        bVals[i] = pos - neg;
    }

    BellCap = newCap;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* a(0) = 1 */
        return (seq_t)1;

    ensure_cache(n);
    return bVals[n] - bVals[n - 1];
}
