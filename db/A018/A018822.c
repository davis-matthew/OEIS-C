/*
 * A018822  n is the sum of k nonzero squares for all 3 <= k <= n-14.
 * https://oeis.org/A018822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) consists of all positive integers N such that for every k
 *            with 3 <= k <= N-14, N can be written as a sum of k non‑zero
 *            integer squares. This property is equivalent to max_{r=14}^{N-3}
 *            ( minCoins[r] + r ) <= N where minCoins[r] is the minimal number
 *            of increments t^2-1 (t ≥ 2) needed to sum t...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  The types seq_t and idx_t are supplied by the build system. */
/* ------------------------------------------------------------ */

static const idx_t INF_IDX = ((~((idx_t)0)) >> 1);   /* large but safe */

/* ---------- dynamic arrays --------------- */

static idx_t *minCoins = NULL;   /* minCoins[r] */
static idx_t *pref     = NULL;   /* max_{t=14..r} (minCoins[t]+t) */
static idx_t  minCoinsSize = 0;  /* number of entries allocated */

static idx_t *incVals = NULL;    /* list of increments t²-1, t ≥ 2 */
static idx_t  incCount = 0;
static idx_t  incCap   = 0;
static idx_t  nextT    = 2;      /* next t to generate increment for */

/* sequence cache ---------------------------------------- */
static idx_t *seq       = NULL;   /* list of terms of A018822 */
static idx_t  seqSize   = 0;
static idx_t  seqCap    = 0;
static idx_t  nextCandidate = 17; /* start from the first known term */

/* ------------------------------------------------------------ */
/* Ensure that increments t²-1 up to needed are available.      */
/* ------------------------------------------------------------ */
static void ensureInc(idx_t needed)
{
    while ((nextT * nextT - 1) <= needed) {
        idx_t inc = nextT * nextT - 1;
        if (incCount == incCap) {
            incCap = incCap ? incCap * 2 : 16;
            incVals = (idx_t *)realloc(incVals, incCap * sizeof(idx_t));
        }
        incVals[incCount++] = inc;
        ++nextT;
    }
}

/* ------------------------------------------------------------ */
/* Extend the DP arrays up to “needed” (inclusive).             */
/* ------------------------------------------------------------ */
static void ensureDP(idx_t needed)
{
    if (needed < minCoinsSize) return;

    idx_t oldSize = minCoinsSize;
    minCoinsSize = needed + 1;

    minCoins = (idx_t *)realloc(minCoins, minCoinsSize * sizeof(idx_t));
    pref     = (idx_t *)realloc(pref,     minCoinsSize * sizeof(idx_t));

    /* initialise base case */
    if (oldSize == 0) {
        minCoins[0] = 0;
        pref[0]     = 0;
        oldSize = 1;
    }

    for (idx_t r = oldSize; r <= needed; ++r) {
        idx_t best = INF_IDX;
        for (idx_t i = 0; i < incCount; ++i) {
            idx_t inc = incVals[i];
            if (inc > r) break;
            if (minCoins[r - inc] != INF_IDX) {
                idx_t cand = minCoins[r - inc] + 1;
                if (cand < best) best = cand;
            }
        }
        minCoins[r] = best;

        pref[r] = pref[r - 1];
        if (r >= 14) {
            idx_t val = best + r;
            if (val > pref[r]) pref[r] = val;
        }
    }
}

/* ------------------------------------------------------------ */
/* Generate terms of the sequence until we have at least n.     */
/* ------------------------------------------------------------ */
static void ensureTerms(idx_t n)
{
    while (seqSize < n) {
        /* make sure we can evaluate the candidate */
        ensureInc(nextCandidate - 3);
        ensureDP (nextCandidate - 3);

        idx_t rMax = nextCandidate - 3;
        if (pref[rMax] <= nextCandidate) {
            if (seqSize == seqCap) {
                seqCap = seqCap ? seqCap * 2 : 32;
                seq = (idx_t *)realloc(seq, seqCap * sizeof(idx_t));
            }
            seq[seqSize++] = nextCandidate;
        }
        ++nextCandidate;
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the problem statement.           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensureTerms(n);
    return (seq_t)seq[n - 1];
}
