/*
 * A208040  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   is_valid_vertical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208040: number of 4 × n 0..1 arrays avoiding
   000 and 010 horizontally and 000 and 101 vertically.
   DP over columns with states (prev,cur) of valid columns. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_valid_vertical.h"

static struct Scratch {
    int m;                 /* number of valid columns */
    int stateCount;        /* m*m */
    int *validCols;        /* list of valid column masks */
    uint16_t *nextMask;    /* allowed next columns per state */
    seq_t *dpPrev;         /* DP for current width */
    seq_t *dpCur;          /* temporary DP */
    seq_t *values;         /* cache of a(k) */
    size_t valuesSize;     /* allocated size of values array */
    idx_t currentMax;      /* largest n cached */
} *S = NULL;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise once */
    if (S == NULL) {
        S = (struct Scratch*)malloc(sizeof(struct Scratch));
        S->m = 0;
        S->stateCount = 0;
        S->validCols = NULL;
        S->nextMask = NULL;
        S->dpPrev = NULL;
        S->dpCur = NULL;
        S->values = NULL;
        S->valuesSize = 0;
        S->currentMax = 0;
    }

    if (S->currentMax == 0) {                /* first call */
        /* enumerate valid columns */
        S->validCols = (int*)malloc(16 * sizeof(int));
        for (int col = 0; col < 16; ++col)
            if (is_valid_vertical(col))
                S->validCols[S->m++] = col;

        S->stateCount = S->m * S->m;

        /* pre‑compute transition masks */
        S->nextMask = (uint16_t*)malloc(S->stateCount * sizeof(uint16_t));
        for (int i = 0; i < S->m; ++i) {
            for (int j = 0; j < S->m; ++j) {
                uint16_t mask = 0;
                for (int k = 0; k < S->m; ++k) {
                    int ok = 1;
                    for (int r = 0; r < 4; ++r) {     /* rows */
                        int b1 = (S->validCols[i] >> r)     & 1;
                        int b2 = (S->validCols[j] >> r)     & 1;
                        int b3 = (S->validCols[k] >> r)     & 1;
                        if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                            (b1 == 0 && b2 == 1 && b3 == 0)) {
                            ok = 0; break;
                        }
                    }
                    if (ok) mask |= (uint16_t)(1u << k);
                }
                int idx = i * S->m + j;
                S->nextMask[idx] = mask;
            }
        }

        /* allocate DP arrays */
        S->dpPrev = (seq_t*)malloc(S->stateCount * sizeof(seq_t));
        S->dpCur  = (seq_t*)malloc(S->stateCount * sizeof(seq_t));

        /* base DP for width = 2: one way for each state */
        for (int s = 0; s < S->stateCount; ++s)
            S->dpPrev[s] = 1;

        /* allocate values cache */
        S->valuesSize = 4;                 /* at least indices 0..3 */
        S->values = (seq_t*)malloc(S->valuesSize * sizeof(seq_t));
        S->values[0] = 0;                   /* unused */
        S->values[1] = (seq_t)S->m;                /* a(1) */
        S->values[2] = (seq_t)S->m * (seq_t)S->m;  /* a(2) */

        S->currentMax = 2;
    }

    /* return cached value if available */
    if (n <= S->currentMax)
        return S->values[n];

    /* extend DP up to requested n */
    for (idx_t width = S->currentMax + 1; width <= n; ++width) {
        /* zero dpCur */
        for (int s = 0; s < S->stateCount; ++s)
            S->dpCur[s] = 0;

        /* transition from width-1 to width */
        for (int s = 0; s < S->stateCount; ++s) {
            seq_t val = S->dpPrev[s];
            if (val == 0) continue;
            int j = s % S->m;               /* second column of state */
            uint16_t mask = S->nextMask[s];
            for (int k = 0; k < S->m; ++k) {
                if (mask & (uint16_t)(1u << k)) {
                    int newState = j * S->m + k;
                    S->dpCur[newState] += val;
                }
            }
        }

        /* swap dp arrays */
        seq_t *tmp = S->dpPrev;
        S->dpPrev = S->dpCur;
        S->dpCur  = tmp;

        /* sum all states for this width */
        seq_t sum = 0;
        for (int s = 0; s < S->stateCount; ++s)
            sum += S->dpPrev[s];

        /* grow values array if necessary */
        if ((size_t)width >= S->valuesSize) {
            size_t newSize = S->valuesSize * 2;
            if (newSize <= (size_t)width) newSize = (size_t)width + 1;
            S->values = (seq_t*)realloc(S->values, newSize * sizeof(seq_t));
            S->valuesSize = newSize;
        }
        S->values[width] = sum;
        S->currentMax = width;
    }

    return S->values[n];
}
