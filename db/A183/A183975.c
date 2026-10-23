/*
 * A183975  Half the number of (n+1)X5 0..3 arrays with no 2X2 subblock sum
 *          differing from a horizontal or vertical neighbor subblock sum by
 *          more than one.
 * https://oeis.org/A183975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = half the number of (n+1)×5 0..3 arrays with no 2×2
 *            subblock sum differing from a * horizontal or vertical neighbor
 *            subblock sum by more than one
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types (defined by the build) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Create all row patterns, group by their 4 pair‑sum vector p,
       and compute horizontal & vertical compatibility.
       ------------------------------------------------------------------*/
    static bool initialized = false;
    static size_t Np = 0;                     /* number of distinct p‑values   */
    static int (*pVals)[4] = NULL;            /* pVals[id][0..3]                */
    static seq_t *mult = NULL;                /* multiplicity of each p          */
    static size_t *horizCounts = NULL;        /* number of horizontal neighbours */
    static int **horizNeighbors = NULL;       /* adjacency list for horizontal   */
    static unsigned char *vertMatrix = NULL;  /* 1 if vertical compatible        */

    if (!initialized) {
        /* --- 1. Enumerate all 5‑digit rows (0..3) and derive p values --- */
        int idCount = 0;
        int tempP[2401][4];
        int tempMult[2401];
        int mapKey[2401];
        for (int i = 0; i < 2401; ++i) mapKey[i] = -1;

        for (int mask = 0; mask < 1024; ++mask) {
            int d[5];
            int t = mask;
            for (int i = 0; i < 5; ++i) { d[i] = t & 3; t >>= 2; }

            int p[4] = { d[0]+d[1], d[1]+d[2], d[2]+d[3], d[3]+d[4] };
            int key = (((p[0]*7)+p[1])*7 + p[2])*7 + p[3];
            int id = mapKey[key];
            if (id == -1) {
                id = idCount;
                mapKey[key] = id;
                tempP[id][0] = p[0];
                tempP[id][1] = p[1];
                tempP[id][2] = p[2];
                tempP[id][3] = p[3];
                tempMult[id] = 1;
                idCount++;
            } else {
                tempMult[id]++;
            }
        }
        Np = idCount;

        pVals = (int (*)[4])calloc(Np, sizeof(*pVals));
        mult  = (seq_t *)calloc(Np, sizeof(*mult));
        for (size_t i = 0; i < Np; ++i) {
            pVals[i][0] = tempP[i][0];
            pVals[i][1] = tempP[i][1];
            pVals[i][2] = tempP[i][2];
            pVals[i][3] = tempP[i][3];
            mult[i] = (seq_t)tempMult[i];
        }

        /* --- 2. Horizontal compatibility graph --- */
        horizCounts = (size_t *)calloc(Np, sizeof(size_t));
        for (size_t i = 0; i < Np; ++i) {
            for (size_t j = 0; j < Np; ++j) {
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    int s1 = pVals[i][k] + pVals[j][k];
                    int s2 = pVals[i][k+1] + pVals[j][k+1];
                    if (s1 > s2+1 || s2 > s1+1) { ok = false; break; }
                }
                if (ok) horizCounts[i]++;
            }
        }
        horizNeighbors = (int **)malloc(Np * sizeof(int *));
        for (size_t i = 0; i < Np; ++i) {
            horizNeighbors[i] = (int *)malloc(horizCounts[i] * sizeof(int));
            size_t idx = 0;
            for (size_t j = 0; j < Np; ++j) {
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    int s1 = pVals[i][k] + pVals[j][k];
                    int s2 = pVals[i][k+1] + pVals[j][k+1];
                    if (s1 > s2+1 || s2 > s1+1) { ok = false; break; }
                }
                if (ok) horizNeighbors[i][idx++] = (int)j;
            }
        }

        /* --- 3. Vertical compatibility matrix --- */
        vertMatrix = (unsigned char *)malloc(Np * Np * sizeof(unsigned char));
        for (size_t i = 0; i < Np; ++i) {
            for (size_t j = 0; j < Np; ++j) {
                bool ok = true;
                for (int k = 0; k < 4; ++k) {
                    int diff = pVals[i][k] - pVals[j][k];
                    if (diff < 0) diff = -diff;
                    if (diff > 1) { ok = false; break; }
                }
                vertMatrix[i*Np + j] = ok ? 1 : 0;
            }
        }
        initialized = true;
    }

    /* ------------------------------------------------------------------
       Dynamic programming over rows.
       ------------------------------------------------------------------*/
    size_t R = (size_t)n + 1U;   /* number of rows in the array */
    if (R < 2U) return 0; /* should not occur for n>=1 */

    size_t totalStates = Np * Np;
    seq_t *arrA = (seq_t *)calloc(totalStates, sizeof(seq_t));
    seq_t *arrB = (seq_t *)calloc(totalStates, sizeof(seq_t));
    size_t *usedA = (size_t *)malloc(totalStates * sizeof(size_t));
    size_t *usedB = (size_t *)malloc(totalStates * sizeof(size_t));
    if (!arrA || !arrB || !usedA || !usedB) return (seq_t)0; /* allocation failure */

    /* Initial states: first two rows must be horizontally compatible */
    size_t usedACount = 0;
    for (size_t prev = 0; prev < Np; ++prev) {
        for (size_t idx = 0; idx < horizCounts[prev]; ++idx) {
            size_t curr = horizNeighbors[prev][idx];
            size_t key = prev * Np + curr;
            arrA[key] = mult[prev] * mult[curr];
            usedA[usedACount++] = key;
        }
    }

    /* Remaining rows */
    size_t steps = (R > 2U) ? (R - 2U) : 0U;
    size_t usedBCount = 0;

    for (size_t step = 0; step < steps; ++step) {
        /* clear arrB entries from previous iteration */
        for (size_t i = 0; i < usedBCount; ++i) arrB[usedB[i]] = 0;
        usedBCount = 0;

        for (size_t i = 0; i < usedACount; ++i) {
            size_t key = usedA[i];
            seq_t count = arrA[key];
            size_t prev = key / Np;
            size_t curr = key % Np;

            for (size_t hIdx = 0; hIdx < horizCounts[curr]; ++hIdx) {
                size_t next = horizNeighbors[curr][hIdx];
                if (!vertMatrix[prev * Np + next]) continue;

                size_t newKey = curr * Np + next;
                if (arrB[newKey] == 0) usedB[usedBCount++] = newKey;
                arrB[newKey] += count * mult[next];
            }
        }

        /* swap arrays and lists for next step */
        seq_t *tmpT = arrA; arrA = arrB; arrB = tmpT;
        size_t *tmpU = usedA; usedA = usedB; usedB = tmpU;
        size_t tmpC = usedACount; usedACount = usedBCount; usedBCount = tmpC;
    }

    /* Sum over final states */
    seq_t total = 0;
    for (size_t i = 0; i < usedACount; ++i) {
        total += arrA[usedA[i]];
    }

    seq_t result = total / (seq_t)2; /* sequence gives half the count */

    /* free memory */
    free(arrA);
    free(arrB);
    free(usedA);
    free(usedB);

    return result;
}
