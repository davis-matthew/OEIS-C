/*
 * A237328  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×5 0..2 arrays satisfying the 2×2‑subblock
 *            adjacency condition. Computed by a row‑by‑row dynamic
 *            programming on pairs of consecutive rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Helpers:   absdiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------  Dependencies  --------------- */
/* No additional headers are needed beyond the standard ones. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/absdiff.h"

/* ---------------  Row enumeration  --------------- */
#define ROWS 243          /* 3^5 possible rows */
static uint8_t rowVals[ROWS][5];   /* rowVals[i][c] ∈ {0,1,2} */

/* Helper: insertion sort of 4 integers in place */
static void sort4(int *v)
{
    for (int i = 1; i < 4; ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
}

/* ---------------  Pair structure  --------------- */
typedef struct {
    uint16_t a, b;            /* indices of the two rows */
    uint8_t f[4];             /* f-values for the pair (range 0..6) */
    uint32_t fcode;           /* base‑7 encoding of f[4] */
} Pair;

/* ---------------  Precomputation data  --------------- */
static bool precomputed = false;
static size_t pairCount = 0;
static Pair *pairArray = NULL;
static size_t *adjSize = NULL;   /* number of outgoing edges for each pair */
static int **adjLists = NULL;    /* adjacency lists (indices of destination pairs) */

/* ---------------  Initialization routine  --------------- */
static void init_precomputation(void)
{
    /* Enumerate all rows */
    for (int i = 0; i < ROWS; ++i) {
        int x = i;
        for (int c = 0; c < 5; ++c) {
            rowVals[i][c] = x % 3;
            x /= 3;
        }
    }

    /* Determine all valid pairs and store them */
    int maxPairs = ROWS * ROWS;   /* maximum possible pairs */
    int *pairIndex = (int*)malloc(maxPairs * sizeof(int));
    for (int i = 0; i < maxPairs; ++i) pairIndex[i] = -1;   /* -1 means invalid */

    /* Temporary array for counting transitions */
    int *tmpAdjCount = (int*)calloc(ROWS * ROWS, sizeof(int));  /* will be resized later */

    /* First pass: collect pairs, compute f, check horizontal condition */
    size_t tempCount = 0;
    Pair *tempPairs = (Pair*)malloc(maxPairs * sizeof(Pair));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int vals[4];
            int f[4];
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int a = rowVals[i][col];
                int b = rowVals[i][col + 1];
                int c = rowVals[j][col];
                int d = rowVals[j][col + 1];
                /* Gather into 4 values and sort */
                vals[0] = a; vals[1] = b; vals[2] = c; vals[3] = d;
                sort4(vals);
                int minv  = vals[0];
                int min2  = vals[1];
                int max2  = vals[2];
                int maxv  = vals[3];
                f[col] = maxv + max2 + min2 - minv;
                /* f ranges 0..6 */
                if (f[col] < 0 || f[col] > 6) ok = false;   /* safety */
            }
            if (!ok) continue;
            for (int col = 0; col < 3; ++col) {
                if (absdiff(f[col], f[col + 1]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            /* Store pair */
            Pair *p = &tempPairs[tempCount];
            p->a = (uint16_t)i;
            p->b = (uint16_t)j;
            p->f[0] = (uint8_t)f[0];
            p->f[1] = (uint8_t)f[1];
            p->f[2] = (uint8_t)f[2];
            p->f[3] = (uint8_t)f[3];
            uint32_t code = 0;
            for (int k = 0; k < 4; ++k) code = code * 7 + f[k];
            p->fcode = code;
            pairIndex[i * ROWS + j] = (int)tempCount;
            ++tempCount;
        }
    }

    /* Now tempCount equals number of valid pairs */
    pairCount = tempCount;
    pairArray = tempPairs;   /* take ownership */
    free(pairIndex);

    /* Allocate adjacency size array */
    adjSize   = (size_t*)calloc(pairCount, sizeof(size_t));

    /* Second pass: count transitions */
    for (size_t p = 0; p < pairCount; ++p) {
        const Pair *pa = &pairArray[p];
        for (size_t q = 0; q < pairCount; ++q) {
            const Pair *pb = &pairArray[q];
            if (pb->a != pa->b) continue;                     /* rows must match */
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (absdiff(pb->f[k], pa->f[k]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            ++adjSize[p];
        }
    }

    /* Allocate adjacency lists */
    adjLists = (int**)malloc(pairCount * sizeof(int*));
    for (size_t p = 0; p < pairCount; ++p) {
        if (adjSize[p] > 0) {
            adjLists[p] = (int*)malloc(adjSize[p] * sizeof(int));
        } else {
            adjLists[p] = NULL;
        }
    }

    /* Third pass: fill adjacency lists */
    size_t *adjPos = (size_t*)calloc(pairCount, sizeof(size_t));
    for (size_t p = 0; p < pairCount; ++p) {
        const Pair *pa = &pairArray[p];
        for (size_t q = 0; q < pairCount; ++q) {
            const Pair *pb = &pairArray[q];
            if (pb->a != pa->b) continue;
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (absdiff(pb->f[k], pa->f[k]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            adjLists[p][adjPos[p]++] = (int)q;
        }
    }
    free(adjPos);
    free(tmpAdjCount);   /* not used; keep for consistency */
    precomputed = true;
}

/* ---------------  Main sequence function  --------------- */
seq_t a(idx_t n)
{
    if (!precomputed) init_precomputation();

    /* DP arrays */
    seq_t *dp = (seq_t*)calloc(pairCount, sizeof(seq_t));
    seq_t *newdp = (seq_t*)calloc(pairCount, sizeof(seq_t));

    /* Initialize: each valid pair can be the first pair */
    for (size_t i = 0; i < pairCount; ++i) dp[i] = 1;

    /* Perform n-1 transitions */
    for (idx_t step = 1; step < n; ++step) {
        /* zero newdp */
        memset(newdp, 0, pairCount * sizeof(seq_t));
        for (size_t p = 0; p < pairCount; ++p) {
            if (dp[p] == 0) continue;
            size_t sz = adjSize[p];
            int *list = adjLists[p];
            for (size_t t = 0; t < sz; ++t) {
                int q = list[t];
                newdp[q] += dp[p];
            }
        }
        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* Sum over all ending pairs */
    seq_t result = 0;
    for (size_t i = 0; i < pairCount; ++i) {
        result += dp[i];
    }

    /* Clean up temporary arrays */
    free(dp);
    free(newdp);

    return result;
}
