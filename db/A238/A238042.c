/*
 * A238042  Number of (n+1)X(4+1) 0..2 arrays with the upper median minus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A238042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute the number of (n+1)×5 arrays over {0,1,2} such that for every
 * 2×2 subblock the difference (upper‑median – lower‑median) differs
 * from the corresponding difference of its horizontal and vertical
 * neighbours by exactly one.  The algorithm uses dynamic programming
 * over pairs of consecutive rows.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 243                /* 3^5 */

typedef struct { int* data; size_t sz, cap; } IntArray;

static int rows[ROWS][5];
static uint8_t diffVec[ROWS][ROWS][4];
static uint8_t validDiff[ROWS][ROWS];
static int stateId[ROWS][ROWS]; /* -1 = not a state */

/* dynamic arrays of states */
static int* stateA = NULL;
static int* stateB = NULL;
static size_t stateCount = 0;

/* transitions */
static IntArray* trans = NULL;

/* DP arrays */
static seq_t* dp = NULL;
static seq_t* newdp = NULL;

static int initialized = 0;

static void init(void)
{
    /* 1. encode all rows */
    for (int i = 0; i < ROWS; ++i) {
        int tmp = i;
        for (int j = 0; j < 5; ++j) {
            rows[i][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* 2. compute difference vectors and validity */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            uint8_t d[4];
            for (int c = 0; c < 4; ++c) {
                uint8_t v[4] = { rows[a][c], rows[a][c+1],
                                 rows[b][c], rows[b][c+1] };
                /* simple insertion sort */
                for (int i = 1; i < 4; ++i)
                    for (int j = i-1; j >= 0 && v[j] > v[j+1]; --j)
                        { uint8_t t = v[j]; v[j] = v[j+1]; v[j+1] = t; }
                d[c] = v[2] - v[1];
            }
            memcpy(diffVec[a][b], d, 4);
            int ok = 1;
            for (int c = 0; c < 3; ++c)
                if (d[c] + 1 != d[c+1] && d[c] - 1 != d[c+1]) { ok = 0; break; }
            validDiff[a][b] = ok;
        }
    }

    /* 3. enumerate valid states and map to ids */
    memset(stateId, 0xff, sizeof(stateId));
    size_t cap = 1024;
    stateA = (int*)malloc(cap * sizeof(int));
    stateB = (int*)malloc(cap * sizeof(int));
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            if (!validDiff[a][b]) continue;
            if (stateCount == cap) {
                cap *= 2;
                stateA = (int*)realloc(stateA, cap * sizeof(int));
                stateB = (int*)realloc(stateB, cap * sizeof(int));
            }
            stateId[a][b] = (int)stateCount;
            stateA[stateCount] = a;
            stateB[stateCount] = b;
            ++stateCount;
        }
    }

    /* 4. build transitions */
    trans = (IntArray*)malloc(stateCount * sizeof(IntArray));
    for (size_t s = 0; s < stateCount; ++s) {
        trans[s].data = NULL;
        trans[s].sz = 0;
        trans[s].cap = 0;
    }

    for (size_t s = 0; s < stateCount; ++s) {
        int a = stateA[s];
        int b = stateB[s];
        uint8_t* prevDiff = diffVec[a][b];
        for (int c = 0; c < ROWS; ++c) {
            if (!validDiff[b][c]) continue;
            uint8_t* currDiff = diffVec[b][c];
            int ok = 1;
            for (int k = 0; k < 4; ++k) {
                int diff = (int)currDiff[k] - (int)prevDiff[k];
                if (diff != 1 && diff != -1) { ok = 0; break; }
            }
            if (!ok) continue;
            int t = stateId[b][c];
            if (t < 0) continue; /* safety */
            /* append t to trans[s] */
            if (trans[s].sz == trans[s].cap) {
                size_t newcap = trans[s].cap ? trans[s].cap * 2 : 4;
                trans[s].data = (int*)realloc(trans[s].data,
                                            newcap * sizeof(int));
                trans[s].cap = newcap;
            }
            trans[s].data[trans[s].sz++] = t;
        }
    }

    /* 5. allocate DP arrays */
    dp = (seq_t*)malloc(stateCount * sizeof(seq_t));
    newdp = (seq_t*)malloc(stateCount * sizeof(seq_t));

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    /* initialise dp with 1 for every state */
    for (size_t i = 0; i < stateCount; ++i) dp[i] = 1;

    if (n > 1) {
        for (idx_t step = 1; step <= n-1; ++step) {
            /* clear newdp */
            for (size_t i = 0; i < stateCount; ++i) newdp[i] = 0;
            for (size_t s = 0; s < stateCount; ++s) {
                if (dp[s] == 0) continue;
                seq_t val = dp[s];
                IntArray* arr = &trans[s];
                for (size_t idx = 0; idx < arr->sz; ++idx)
                    newdp[arr->data[idx]] += val;
            }
            /* swap dp and newdp */
            seq_t* tmp = dp;
            dp = newdp;
            newdp = tmp;
        }
    }

    seq_t ans = 0;
    for (size_t i = 0; i < stateCount; ++i) ans += dp[i];
    return ans;
}
