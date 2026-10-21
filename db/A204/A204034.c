/*
 * A204034  Number of (n+1)X4 0..1 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 18, u64 n <= 50, u128 n <= 114,
 *            big any n
 * Helpers:   key3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204034 – Number of (n+1)×4 0..1 arrays with the sums of
    2×2 subblocks nondecreasing rightwards (horizontal) and downwards
    (vertical).  Implementation uses a finite‑state DP over column
    masks.  The DP state is (prevSumIdx, curMask).  For each n the
    transition is applied n‑1 times starting from all valid 2×4 base
    blocks.  The DP runs in O(n·S·16) time where S≤125 is the number
    of distinct 3‑tuple sum vectors. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/key3.h"

/*  External types are provided by the build system.  No redefinition
    is required.  */
 
/*  -----  Precomputation tables  -----  */

static int  sumIdx[16][16];          /* index of sum vector for a pair (a,b), -1 if horizontal condition fails */
static int  sumVec[125][3];          /* unique sum vectors (0..4 each) */
static int   S;                      /* number of unique sum vectors */
struct TransItem
{
    uint8_t next;     /* next column mask                */
    uint8_t newIdx;   /* index of sum vector of pair (cur,next) */
};
static struct TransItem **trans;
static size_t *transSize;
static bool initialized = false;

/*  initialise all tables – executed only once */
static void init_tables(void)
{
    if (initialized) return;

    /*  1. build all sum vectors that satisfy the horizontal
        nondecreasing condition                           */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            sumIdx[i][j] = -1;
    S = 0;
    for (int a = 0; a < 16; ++a)
    {
        for (int b = 0; b < 16; ++b)
        {
            int s[3];
            bool ok = true;
            for (int k = 0; k < 3; ++k)
            {
                int val = ((a >> k) & 1) + ((a >> (k+1)) & 1) +
                          ((b >> k) & 1) + ((b >> (k+1)) & 1);
                s[k] = val;
                if (k > 0 && s[k-1] > s[k]) ok = false;
            }
            if (!ok) continue;
            int key = key3(s[0], s[1], s[2]);
            int idx = -1;
            for (int t = 0; t < S; ++t)
            {
                if (sumVec[t][0] == s[0] &&
                    sumVec[t][1] == s[1] &&
                    sumVec[t][2] == s[2]) { idx = t; break; }
            }
            if (idx < 0) { idx = S; S++; }
            sumVec[idx][0] = s[0];
            sumVec[idx][1] = s[1];
            sumVec[idx][2] = s[2];
            sumIdx[a][b] = idx;
        }
    }

    /*  2. precompute all allowed vertical transitions */
    size_t totalStates = (size_t)S * 16;
    trans     = (struct TransItem **)malloc(totalStates * sizeof(struct TransItem *));
    transSize = (size_t *)malloc(totalStates * sizeof(size_t));
    for (size_t st = 0; st < totalStates; ++st)
    {
        trans[st] = (struct TransItem *)malloc(16 * sizeof(struct TransItem));
        transSize[st] = 0;
    }

    for (int prev = 0; prev < S; ++prev)
    {
        for (int cur = 0; cur < 16; ++cur)
        {
            size_t pos = (size_t)prev * 16 + cur;
            size_t cnt = 0;
            for (int next = 0; next < 16; ++next)
            {
                int nextIdx = sumIdx[cur][next];
                if (nextIdx < 0) continue;   /* horizontal condition fails */
                bool v_ok = true;
                for (int k = 0; k < 3; ++k)
                {
                    if (sumVec[prev][k] > sumVec[nextIdx][k]) { v_ok = false; break; }
                }
                if (!v_ok) continue;
                trans[pos][cnt].next   = (uint8_t)next;
                trans[pos][cnt].newIdx = (uint8_t)nextIdx;
                cnt++;
            }
            transSize[pos] = cnt;
        }
    }

    initialized = true;
}

/*  -----  main sequence function  -----  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – undefined for n=0 */

    init_tables();   /* ensure tables are ready */

    /*  number of DP states */
    static size_t stateCount = 0;
    if (stateCount == 0) stateCount = (size_t)S * 16;

    /*  DP arrays – static so they survive successive calls */
    static seq_t *dp     = NULL;
    static seq_t *dp_next = NULL;
    if (!dp)
    {
        dp      = (seq_t *)calloc(stateCount, sizeof(seq_t));
        dp_next = (seq_t *)calloc(stateCount, sizeof(seq_t));
    }
    else
    {
        memset(dp,      0, stateCount * sizeof(seq_t));
        memset(dp_next, 0, stateCount * sizeof(seq_t));
    }

    /*  initialise with all admissible 2×4 base blocks
        (i.e., first two columns)                         */
    seq_t ONE = 1;
    for (int a_mask = 0; a_mask < 16; ++a_mask)
    for (int b_mask = 0; b_mask < 16; ++b_mask)
    {
        int idx = sumIdx[a_mask][b_mask];
        if (idx < 0) continue;
        size_t st = (size_t)idx * 16 + b_mask;
        dp[st] += ONE;
    }

    /*  apply transitions n-1 times (n columns in total) */
    for (idx_t step = 1; step < n; ++step)
    {
        for (size_t st = 0; st < stateCount; ++st)
        {
            seq_t val = dp[st];
            if (!val) continue;
            size_t tcnt = transSize[st];
            struct TransItem *tptr = trans[st];
            for (size_t k = 0; k < tcnt; ++k)
            {
                size_t newState = (size_t)tptr[k].newIdx * 16 + tptr[k].next;
                dp_next[newState] += val;
            }
        }
        /*  swap arrays and clear next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
        memset(dp_next, 0, stateCount * sizeof(seq_t));
    }

    /*  sum over all final states */
    seq_t result = 0;
    for (size_t st = 0; st < stateCount; ++st) result += dp[st];

    return result;
}
