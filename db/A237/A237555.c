/*
 * A237555  Number of (n+1)X(6+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237555: number of (n+1)×7 0..2 arrays satisfying the
   “maximum minus upper median” condition for every 2×2 subblock
   with adjacency difference equal to 1.  The values are computed
   by dynamic programming over the finite set of admissible
   2×2-difference patterns (at most 16). */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Fundamental types provided by the build environment                 */
/* --------------------------------------------------------------------- */

/* seq_t is an unsigned integer type or arbitrary‑precision class;
   idx_t is an unsigned 64‑bit index (uint64_t).  No definitions here. */

/* --------------------------------------------------------------------- */
/*  Helper structures for dynamic arrays (compatible with C++17)         */
/* --------------------------------------------------------------------- */

/* Edge between two rows together with the id of the difference pattern. */
typedef struct { int dest; int diff; } EdgePair;

typedef struct {
    EdgePair *data;
    size_t sz;
    size_t cap;
} EdgeVec;

static void initEdgeVec(EdgeVec *v) { v->data = NULL; v->sz = 0; v->cap = 0; }
static void pushEdgeVec(EdgeVec *v, EdgePair e) {
    if (v->sz == v->cap) {
        size_t newcap = v->cap ? v->cap << 1 : 4;
        v->data = (EdgePair*)realloc(v->data, newcap * sizeof(EdgePair));
        v->cap = newcap;
    }
    v->data[v->sz++] = e;
}

typedef struct {
    int *data;
    size_t sz;
    size_t cap;
} IntVec;

static void initIntVec(IntVec *v) { v->data = NULL; v->sz = 0; v->cap = 0; }
static void pushIntVec(IntVec *v, int x) {
    if (v->sz == v->cap) {
        size_t newcap = v->cap ? v->cap << 1 : 4;
        v->data = (int*)realloc(v->data, newcap * sizeof(int));
        v->cap = newcap;
    }
    v->data[v->sz++] = x;
}

/* --------------------------------------------------------------------- */
/*  Global tables                                                     */
/* --------------------------------------------------------------------- */
#define NUM_ROWS 2187              /* 3^7 possible rows */
#define NUM_DIFFS 16               /* maximum number of distinct diff patterns */

static uint8_t  rowVals[NUM_ROWS][7];          /* each row encoded in base‑3 */
static EdgeVec  edgesFrom[NUM_ROWS];
static EdgeVec  edgesFromRev[NUM_ROWS];       /* reverse edges */
static IntVec   transitions[NUM_ROWS * NUM_DIFFS];

static int diffPatternToID[729];               /* –1 if not seen yet     */
static int diffPatterns[NUM_DIFFS][6];         /* stored pattern if seen */
static bool verticalAdj[NUM_DIFFS][NUM_DIFFS]; /* vertical adjacency?   */
static int  diffCount = 0;
static bool initialized = false;

/* --------------------------------------------------------------------- */
/*  Build all tables the first time we are asked for a value.           */
/* --------------------------------------------------------------------- */
static void build_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* ----- encode all possible rows ---------------------------------- */
    for (int code = 0; code < NUM_ROWS; ++code) {
        int x = code;
        for (int i = 6; i >= 0; --i) {
            rowVals[code][i] = x % 3;
            x /= 3;
        }
    }

    /* ----- initialise helpers ---------------------------------------- */
    for (int i = 0; i < 729; ++i) diffPatternToID[i] = -1;
    for (int i = 0; i < NUM_ROWS; ++i) {
        initEdgeVec(&edgesFrom[i]);
        initEdgeVec(&edgesFromRev[i]);
    }

    /* ----- discover all valid diff patterns and build adjacency edges -- */
    int nextDiffID = 0;
    int out[6];
    for (int a = 0; a < NUM_ROWS; ++a) {
        for (int b = 0; b < NUM_ROWS; ++b) {
            /* compute the six differences for this pair */
            for (int j = 0; j < 6; ++j) {
                int v0 = rowVals[a][j];
                int v1 = rowVals[a][j+1];
                int v2 = rowVals[b][j];
                int v3 = rowVals[b][j+1];

                /* sort the four values (simple insertion sort) */
                if (v0 > v1) { int t=v0; v0=v1; v1=t; }
                if (v2 > v3) { int t=v2; v2=v3; v3=t; }
                if (v0 > v2) { int t=v0; v0=v2; v2=t; }
                if (v1 > v3) { int t=v1; v1=v3; v3=t; }
                if (v1 > v2) { int t=v1; v1=v2; v2=t; }

                out[j] = v3 - v2;     /* 0,1,2 possible */
            }

            /* check horizontal adjacency: differences must differ by 1 */
            bool horiz_ok = true;
            for (int j = 0; j < 5; ++j) {
                int d = (out[j] > out[j+1]) ? out[j]-out[j+1] : out[j+1]-out[j];
                if (d != 1) { horiz_ok = false; break; }
            }
            if (!horiz_ok) continue;

            /* encode the 6‑digit base‑3 code of the pattern */
            int code = 0;
            for (int j = 0; j < 6; ++j) code = code * 3 + out[j];

            int diffID = diffPatternToID[code];
            if (diffID == -1) {           /* new pattern discovered */
                diffID = nextDiffID++;
                diffPatternToID[code] = diffID;
                for (int j = 0; j < 6; ++j) diffPatterns[diffID][j] = out[j];
            }

            /* record the directed edge a -> b with this pattern */
            EdgePair e  = { b, diffID };
            pushEdgeVec(&edgesFrom[a], e);
            EdgePair e_rev = { a, diffID };
            pushEdgeVec(&edgesFromRev[b], e_rev);
        }
    }
    diffCount = nextDiffID;  /* number of concrete diff patterns discovered */

    /* ----- vertical adjacency between difference patterns ----------- */
    for (int i = 0; i < diffCount; ++i) {
        for (int j = 0; j < diffCount; ++j) {
            bool ok = true;
            for (int k = 0; k < 6; ++k) {
                int d = (diffPatterns[i][k] > diffPatterns[j][k])
                        ? diffPatterns[i][k] - diffPatterns[j][k]
                        : diffPatterns[j][k] - diffPatterns[i][k];
                if (d != 1) { ok = false; break; }
            }
            verticalAdj[i][j] = ok;
        }
    }

    /* ----- build transitions for each state (prevRow, prevDiff) ------ */
    int stateCount = NUM_ROWS * diffCount;
    for (int i = 0; i < stateCount; ++i) initIntVec(&transitions[i]);

    for (int r = 0; r < NUM_ROWS; ++r) {
        EdgeVec *src = &edgesFrom[r];
        for (size_t epos = 0; epos < src->sz; ++epos) {
            int dest   = src->data[epos].dest;
            int diffC  = src->data[epos].diff;
            for (int dPrev = 0; dPrev < diffCount; ++dPrev) {
                if (!verticalAdj[dPrev][diffC]) continue;
                int statePrev = r * diffCount + dPrev;
                int stateNext = dest * diffCount + diffC;
                pushIntVec(&transitions[statePrev], stateNext);
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function a(n)                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    build_tables();

    if (n < 1) return (seq_t)0;

    int stateCount = NUM_ROWS * diffCount;
    seq_t *curr  = (seq_t*)calloc(stateCount, sizeof(seq_t));
    seq_t *next  = (seq_t*)calloc(stateCount, sizeof(seq_t));

    /* ---- initial counts: all (row0,row1) pairs with a valid diff pattern */
    for (int r = 0; r < NUM_ROWS; ++r) {
        EdgeVec *rev = &edgesFromRev[r];
        for (size_t k = 0; k < rev->sz; ++k) {
            int diffID = rev->data[k].diff;
            int state  = r * diffCount + diffID;
            curr[state] += (seq_t)1;
        }
    }

    /* ----- dynamic programming: add rows 2 .. n --------------------- */
    for (idx_t step = 1; step < n; ++step) {
        memset(next, 0, stateCount * sizeof(seq_t));
        for (int s = 0; s < stateCount; ++s) {
            seq_t cnt = curr[s];
            if (cnt == 0) continue;
            IntVec *vec = &transitions[s];
            for (size_t k = 0; k < vec->sz; ++k) {
                next[vec->data[k]] += cnt;
            }
        }
        seq_t *tmp = curr; curr = next; next = tmp; /* swap */
    }

    /* ---- sum all final states ------------------------------------- */
    seq_t result = (seq_t)0;
    for (int s = 0; s < stateCount; ++s) result += curr[s];

    free(curr);
    free(next);
    return result;
}
