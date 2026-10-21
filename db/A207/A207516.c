/*
 * A207516  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A207516.  It builds an automaton on the
   14 admissible 5‑bit rows (rows that avoid the horizontal patterns
   001 and 010).  Each state represents a pair of consecutive rows.
   A transition from (a,b) to (b,c) is allowed iff no column
   contains the vertical forbidden patterns 010 or 101.  The number of
   arrays of length n is the number of walks of length n‑2 in this
   directed graph.  The DP below runs in O(n · STATE_COUNT · dest) time
   and uses only integer arithmetic. */
#include <stdbool.h>
#include <stdint.h>

#define ALLOWED_ROWS_COUNT 14
#define STATE_COUNT (ALLOWED_ROWS_COUNT * ALLOWED_ROWS_COUNT)

/* Main entry for the sequence.  idx_t and seq_t are provided by the
   build system. */
seq_t a(idx_t n)
{
    /* Small cases. */
    if (n == (idx_t)1) return (seq_t)14;
    if (n == (idx_t)2) return (seq_t)196;

    /* All admissible 5‑bit rows.  14 rows produce the horizontal
       constraints 001 and 010. */
    int allowedRows[ALLOWED_ROWS_COUNT];
    int cnt = 0;
    for (int mask = 0; mask < 32; ++mask) {
        bool ok = true;
        for (int j = 0; j <= 2; ++j) {
            int b0 = (mask >> j) & 1;
            int b1 = (mask >> (j + 1)) & 1;
            int b2 = (mask >> (j + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 0 && b1 == 1 && b2 == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) allowedRows[cnt++] = mask;
    }

    /* Transition graph: for each state s = (a,b) we store all
       destinations dest = (b,c) that are allowed.  destCount[s] tells
       how many destinations exist. */
    uint8_t destCount[STATE_COUNT];
    uint16_t destList[STATE_COUNT][ALLOWED_ROWS_COUNT];

    for (int a = 0; a < ALLOWED_ROWS_COUNT; ++a) {
        for (int b = 0; b < ALLOWED_ROWS_COUNT; ++b) {
            int s = a * ALLOWED_ROWS_COUNT + b;
            destCount[s] = 0;
            for (int c = 0; c < ALLOWED_ROWS_COUNT; ++c) {
                bool ok = true;
                for (int col = 0; col < 5; ++col) {
                    int bitA = (allowedRows[a] >> col) & 1;
                    int bitB = (allowedRows[b] >> col) & 1;
                    int bitC = (allowedRows[c] >> col) & 1;
                    if ((bitA == 0 && bitB == 1 && bitC == 0) ||
                        (bitA == 1 && bitB == 0 && bitC == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    destList[s][destCount[s]++] =
                        b * ALLOWED_ROWS_COUNT + c;
                }
            }
        }
    }

    /* DP arrays for the current and next row counts. */
    seq_t dpCur[STATE_COUNT];
    seq_t dpNext[STATE_COUNT];

    /* Base case: all 2‑row arrays are admissible. */
    for (int i = 0; i < STATE_COUNT; ++i) dpCur[i] = (seq_t)1;

    /* Extend the array one row at a time. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i) dpNext[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dpCur[s];
            if (val == (seq_t)0) continue;
            int dcnt = destCount[s];
            for (int k = 0; k < dcnt; ++k) {
                uint16_t dest = destList[s][k];
                dpNext[dest] += val;
            }
        }

        /* Prepare for next iteration. */
        for (int i = 0; i < STATE_COUNT; ++i) dpCur[i] = dpNext[i];
    }

    /* Sum over all final states to get the total number of arrays. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) result += dpCur[i];
    return result;
}
