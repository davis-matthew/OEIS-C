/*
 * A207763  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207763: 3 × n 0..1 arrays avoiding
   - horizontally 000 and 101
   - vertically 011 and 101
   Recurrence by DP over the last two columns. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;      /* cache[i] = a(i+1) */
    static idx_t maxComputed = 0;    /* highest n for which a(n) is cached */

    /* Transition matrix and auxiliary data */
    static bool initialized = false;
    static bool trans[36][36] = {0};
    static int allowed[6];
    static int idxMap[8];
    static int numAllowed = 0;
    static seq_t stateCur[36];
    static seq_t stateNext[36];

    /* Initialise once */
    if (!initialized) {
        for (int i = 0; i < 8; ++i) idxMap[i] = -1;
        numAllowed = 0;
        for (int v = 0; v < 8; ++v) {
            if (v == 0b011 || v == 0b101) continue;   /* vertical forbidden */
            idxMap[v] = numAllowed;
            allowed[numAllowed++] = v;
        }
        for (int i = 0; i < 36; ++i)
            for (int j = 0; j < 36; ++j)
                trans[i][j] = false;
        for (int i = 0; i < numAllowed; ++i) {
            for (int j = 0; j < numAllowed; ++j) {
                int colA = allowed[i];
                int colB = allowed[j];
                int src = i * 6 + j;   /* state (A,B) */
                for (int k = 0; k < numAllowed; ++k) {
                    int colC = allowed[k];
                    bool ok = true;
                    for (int r = 0; r < 3; ++r) {
                        int a = (colA >> r) & 1;
                        int b = (colB >> r) & 1;
                        int c = (colC >> r) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 1 && b == 0 && c == 1))
                            { ok = false; break; }
                    }
                    if (!ok) continue;
                    int dst = j * 6 + k;   /* state (B,C) */
                    trans[src][dst] = true;
                }
            }
        }
        initialized = true;
    }

    /* Expand cache up to n, if necessary */
    if (n > maxComputed) {
        size_t newSize = (size_t)n;
        cache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));

        for (idx_t len = maxComputed + 1; len <= n; ++len) {
            if (len == 1) {
                cache[0] = (seq_t)6;
                /* stateCur is not used for len==1 */
            } else if (len == 2) {
                cache[1] = (seq_t)36;
                for (int s = 0; s < 36; ++s) stateCur[s] = 1; /* all (A,B) */
            } else {
                /* Compute next state counts from stateCur */
                for (int s = 0; s < 36; ++s) stateNext[s] = 0;
                for (int s1 = 0; s1 < 36; ++s1) {
                    seq_t val = stateCur[s1];
                    if (val == 0) continue;
                    for (int s2 = 0; s2 < 36; ++s2) {
                        if (!trans[s1][s2]) continue;
                        stateNext[s2] += val;
                    }
                }
                /* Copy stateNext to stateCur for next iteration */
                for (int s = 0; s < 36; ++s) stateCur[s] = stateNext[s];

                /* Sum all state counts to obtain a(len) */
                seq_t sum = 0;
                for (int s = 0; s < 36; ++s) sum += stateCur[s];
                cache[(size_t)len - 1] = sum;
            }
        }
        maxComputed = n;
    }

    return cache[(size_t)n - 1];
}
