/*
 * A196142  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,0,4,1,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   decode_code, encode_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A196142 – Number of n×4 0..4 arrays with neighbour conditions
 *  The algorithm uses dynamic programming over rows.  For a row of 4 cells
 *  (values 0..4) the state is:
 *      - the 4 values (0..4 each) – 5⁴ = 625 possibilities
 *      - the count of already seen neighbour cells (horizontal + up)
 *        that are equal to the target value of each cell – each 0..4
 *  A state is represented by (rowIndex * 625 + countsCode) where countsCode
 *  is a base‑5 number of the 4 counts.  The DP iterates over rows, adding
 *  a new row and ensuring every cell of the previous row satisfies the
 *  required number of equal neighbours.  Only transitions that meet the
 *  requirement are kept – each state has on average about 39 possible
 *  next rows, so the algorithm runs in roughly O(n) time for n up to 10 000.
 *  The sequence values are returned as type seq_t (any unsigned integer or
 *  arbitrary‑precision integer provided by the build).                     */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/decode_code.h"
#include "../utils/encode_counts.h"

/* constants */
#define ROWS     625          /* 5⁴ */
#define STATES   (ROWS * ROWS)/* 390 625 */

/* global tables */
static unsigned char valDigits[ROWS][4];          /* row value digits */
static unsigned char targetVals[ROWS][4];         /* target for each cell */
static unsigned char horCnt[ROWS][4];             /* horizontal neighbour counts */

/* precomputed code → counts */
static unsigned char codeCounts[ROWS][4];         /* counts from code */

/* transition lists: for each state the list of allowed next row indices */
static uint16_t  *nextRows;      /* concatenated lists */
static size_t    *nextStart;     /* index of start of list for each state (size STATES+1) */

/* function prototypes for internal helpers */
static void precompute_tables(void);
static void precompute_transitions(void);

/* ------------------------------------------------------------------- */
static void precompute_tables(void)
{
    const unsigned char targetOf[5] = {3, 0, 4, 1, 2};
    /* generate all 625 row value combinations */
    for (int v = 0; v < ROWS; ++v) {
        int t = v;
        for (int j = 0; j < 4; ++j) {
            valDigits[v][j]     = (unsigned char)(t % 5);
            targetVals[v][j]    = targetOf[valDigits[v][j]];
            t /= 5;
        }
        /* compute horizontal neighbour counts for this row */
        for (int j = 0; j < 4; ++j) {
            unsigned char cnt = 0;
            unsigned char tgt = targetVals[v][j];
            if (j > 0 && valDigits[v][j-1] == tgt) ++cnt;
            if (j < 3 && valDigits[v][j+1] == tgt) ++cnt;
            horCnt[v][j] = cnt;
        }
    }
    /* precompute code → counts */
    for (int code = 0; code < ROWS; ++code) {
        decode_code((size_t)code, codeCounts[code]);
    }
}

/* ------------------------------------------------------------------- */
static void precompute_transitions(void)
{
    /* first pass: count how many next rows each state allows */
    size_t *cntPtr = (size_t*)malloc(sizeof(size_t) * STATES);
    size_t totalAllowed = 0;
    const unsigned char targetOf[5] = {3, 0, 4, 1, 2};

    for (int s = 0; s < STATES; ++s) {
        int prevIdx = s / ROWS;
        int countsCode = s % ROWS;
        const unsigned char *prevCounts = codeCounts[countsCode];
        const unsigned char *prevDigits = valDigits[prevIdx];

        int need[4];
        bool ok = true;
        for (int j = 0; j < 4; ++j) {
            int n = (int)prevDigits[j] - (int)prevCounts[j];
            if (n < 0 || n > 1) { ok = false; break; }
            need[j] = n;
        }
        if (!ok) {
            cntPtr[s] = 0;
            continue;
        }

        size_t ct = 0;
        for (int nxt = 0; nxt < ROWS; ++nxt) {
            bool pat = true;
            const unsigned char *nxtDigits = valDigits[nxt];
            for (int j = 0; j < 4; ++j) {
                bool equal = (nxtDigits[j] == targetOf[prevDigits[j]]);
                if (equal != (need[j] == 1)) { pat = false; break; }
            }
            if (pat) ++ct;
        }
        cntPtr[s] = ct;
        totalAllowed += ct;
    }

    /* allocate transition storage */
    nextStart = (size_t*)malloc(sizeof(size_t) * (STATES + 1));
    nextRows  = (uint16_t*)malloc(sizeof(uint16_t) * totalAllowed);
    nextStart[0] = 0;
    for (int s = 0; s < STATES; ++s) nextStart[s+1] = nextStart[s] + cntPtr[s];

    /* second pass: fill the lists */
    for (int s = 0; s < STATES; ++s) {
        int prevIdx = s / ROWS;
        const unsigned char *prevDigits = valDigits[prevIdx];
        size_t pos = nextStart[s];
        for (int nxt = 0; nxt < ROWS; ++nxt) {
            bool pat = true;
            const unsigned char *nxtDigits = valDigits[nxt];
            for (int j = 0; j < 4; ++j) {
                bool equal = (nxtDigits[j] == targetOf[prevDigits[j]]);
                if (equal != (nxtDigits[j] == targetOf[prevDigits[j]])) { /* placeholder */ }
                /* actual check performed below */
            }
            /* Determine required equality pattern */
            unsigned char need[4];
            for (int j = 0; j < 4; ++j) {
                int n = (int)valDigits[prevIdx][j] - (int)codeCounts[s % ROWS][j];
                need[j] = (unsigned char)n;
            }
            for (int j = 0; j < 4; ++j) {
                bool equal = (nxtDigits[j] == targetOf[prevDigits[j]]);
                if (equal != (need[j] == 1)) { pat = false; break; }
            }
            if (pat) nextRows[pos++] = (uint16_t)nxt;
        }
    }

    free(cntPtr);
}

/* ------------------------------------------------------------------- */
static void init_if_needed(void)
{
    static bool initialized = false;
    if (initialized) return;
    precompute_tables();
    precompute_transitions();
    initialized = true;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static buffers for incremental computation */
    static seq_t *dp = NULL;
    static seq_t *ndp = NULL;
    static seq_t *ans = NULL;      /* array of computed a(k), 1‑based index */
    static idx_t computed = 0;     /* number of a(k) already computed */

    init_if_needed();

    /* expand computation up to desired n */
    if (n > computed) {
        /* if first time, allocate buffers */
        if (dp == NULL) {
            dp  = (seq_t*)calloc(STATES, sizeof(seq_t));
            ndp = (seq_t*)calloc(STATES, sizeof(seq_t));
        }
        /* allocate answer array */
        ans = (seq_t*)realloc(ans, sizeof(seq_t) * n);
        /* first row initialization if we have not computed anything yet */
        if (computed == 0) {
            /* initialise dp with first row states */
            for (int rowIdx = 0; rowIdx < ROWS; ++rowIdx) {
                const unsigned char *counts = horCnt[rowIdx];
                /* ensure horizontal counts do not exceed value */
                bool ok = true;
                for (int j = 0; j < 4; ++j)
                    if (counts[j] > valDigits[rowIdx][j]) { ok = false; break; }
                if (!ok) continue;
                size_t key = (size_t)rowIdx * ROWS + encode_counts(counts);
                dp[key] = (seq_t)1;
            }
            /* compute answer for n = 1 */
            seq_t sum = (seq_t)0;
            for (size_t s = 0; s < STATES; ++s) {
                if (dp[s] == 0) continue;
                int prevIdx = s / ROWS;
                int countsCode = s % ROWS;
                const unsigned char *counts = codeCounts[countsCode];
                const unsigned char *digits = valDigits[prevIdx];
                bool ok = true;
                for (int j = 0; j < 4; ++j)
                    if (counts[j] != digits[j]) { ok = false; break; }
                if (ok) sum += dp[s];
            }
            ans[0] = sum;
            computed = 1;
        }
        /* iterate to compute up to n */
        for (idx_t cur = computed + 1; cur <= n; ++cur) {
            /* clear ndp */
            memset(ndp, 0, sizeof(seq_t) * STATES);

            /* transition from dp to ndp */
            for (size_t s = 0; s < STATES; ++s) {
                seq_t ways = dp[s];
                if (ways == 0) continue;
                int prevIdx = s / ROWS;
                const unsigned char *prevDigits = valDigits[prevIdx];

                size_t start = nextStart[s];
                size_t end   = nextStart[s + 1];
                for (size_t p = start; p < end; ++p) {
                    int nxtIdx = nextRows[p];
                    /* compute nextCounts */
                    unsigned char nextCounts[4];
                    for (int j = 0; j < 4; ++j) {
                        unsigned char upEqual = (prevDigits[j] == targetVals[nxtIdx][j]) ? 1 : 0;
                        nextCounts[j] = horCnt[nxtIdx][j] + upEqual;
                    }
                    size_t key = (size_t)nxtIdx * ROWS + encode_counts(nextCounts);
                    ndp[key] += ways;
                }
            }
            /* swap dp and ndp pointers */
            seq_t *tmp = dp; dp = ndp; ndp = tmp;

            /* compute answer for current row count cur */
            seq_t sum = (seq_t)0;
            for (size_t s = 0; s < STATES; ++s) {
                if (dp[s] == 0) continue;
                int prevIdx = s / ROWS;
                int countsCode = s % ROWS;
                const unsigned char *counts = codeCounts[countsCode];
                const unsigned char *digits = valDigits[prevIdx];
                bool ok = true;
                for (int j = 0; j < 4; ++j)
                    if (counts[j] != digits[j]) { ok = false; break; }
                if (ok) sum += dp[s];
            }
            ans[cur - 1] = sum;   /* store at 0‑based index */
        }
        computed = n;
    }
    /* return the requested value */
    return ans[n - 1];
}
