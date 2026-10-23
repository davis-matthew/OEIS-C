/*
 * A188760  Number of nX6 binary arrays without the pattern 0 0 1 vertically
 *          or horizontally.
 * https://oeis.org/A188760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A188760: Number of n×6 binary arrays with no 0 0 1 pattern
 * horizontally (within a row) or vertically (within a column).
 *
 * Recurrence:
 *   a(1) = 33
 *   a(2) = 1089
 *   For n ≥ 3:
 *     Let V be the set of all 6‑bit rows that avoid 001 horizontally
 *     (|V| = 33).  For each pair (r1,r2) ∈ V×V let
 *     ALLOW(r1,r2) be the set of rows r3 ∈ V that together with
 *     r1,r2 do not contain 001 vertically.
 *     Then for n ≥ 3
 *       a(n) = Σ_{r1,r2∈V} Σ_{r3∈ALLOW(r1,r2)} a(n‑1,r1,r2)
 *     where a(k,r1,r2) is the number of length‑k sequences of rows
 *     ending with rows r1,r2.  The implementation below uses DP
 *     over the pair of last two rows.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t  validRows[64];
static int      validCount;
static uint64_t allowMask[64][64];
static int      initialized = 0;

/* pre‑compute all rows that avoid 001 horizontally,
   and the masks of rows that are compatible vertically. */
static void init_sequence(void)
{
    if (initialized) return;

    int cnt = 0;
    for (int row = 0; row < 64; ++row) {
        /* check horizontally: no three consecutive 0 0 1 */
        int hor_ok = 1;
        for (int j = 0; j <= 3; ++j) {
            if (((row >> j) & 7) == 1) { /* pattern 0 0 1 */
                hor_ok = 0;
                break;
            }
        }
        if (hor_ok) {
            validRows[cnt++] = (uint8_t)row;
        }
    }
    validCount = cnt;            /* Should be 33 */

    /* pre‑compute vertical compatibility */
    for (int i = 0; i < validCount; ++i) {
        uint8_t a = validRows[i];
        for (int j = 0; j < validCount; ++j) {
            uint8_t b = validRows[j];
            uint64_t mask = 0;
            for (int k = 0; k < validCount; ++k) {
                uint8_t c = validRows[k];
                int vert_ok = 1;
                for (int bit = 0; bit < 6; ++bit) {
                    int a0 = (a >> bit) & 1;
                    int b0 = (b >> bit) & 1;
                    int c0 = (c >> bit) & 1;
                    if (!a0 && !b0 && c0) { /* 0 0 1 vertically */
                        vert_ok = 0;
                        break;
                    }
                }
                if (vert_ok) {
                    mask |= ((uint64_t)1) << k;
                }
            }
            allowMask[i][j] = mask;
        }
    }

    initialized = 1;
}

/*
 * Main sequence function; external linkage required.
 * n is at least 1.
 */
seq_t a(idx_t n)
{
    init_sequence();

    if (n == 1) return (seq_t)validCount;
    if (n == 2) return (seq_t)validCount * (seq_t)validCount;

    size_t V = (size_t)validCount;
    size_t matrixSize = V * V;

    /* allocate DP arrays */
    seq_t *dpPrev = (seq_t*)malloc(matrixSize * sizeof(seq_t));
    seq_t *dpCur  = (seq_t*)malloc(matrixSize * sizeof(seq_t));
    if (!dpPrev || !dpCur) { /* allocation failure; return 0 */
        free(dpPrev);
        free(dpCur);
        return (seq_t)0;
    }
    memset(dpPrev, 0, matrixSize * sizeof(seq_t));
    memset(dpCur, 0, matrixSize * sizeof(seq_t));

    /* initialise for length 2: each pair of rows is allowed once */
    for (size_t idx = 0; idx < matrixSize; ++idx) {
        dpPrev[idx] = (seq_t)1;
    }

    for (idx_t step = 3; step <= n; ++step) {
        memset(dpCur, 0, matrixSize * sizeof(seq_t));

        for (size_t i = 0; i < V; ++i) {
            for (size_t j = 0; j < V; ++j) {
                seq_t val = dpPrev[i * V + j];
                if (val == 0) continue;

                uint64_t mask = allowMask[i][j];
                while (mask) {
                    uint64_t low = mask & -mask;
                    int k = __builtin_ctzll(mask);
                    dpCur[j * V + k] += val;
                    mask ^= low;
                }
            }
        }

        /* swap dpPrev and dpCur for next iteration */
        seq_t *tmp = dpPrev;
        dpPrev = dpCur;
        dpCur = tmp;
    }

    /* sum over all ending states to get a(n) */
    seq_t sum = 0;
    for (size_t idx = 0; idx < matrixSize; ++idx) {
        sum += dpPrev[idx];
    }

    free(dpPrev);
    free(dpCur);
    return sum;
}
