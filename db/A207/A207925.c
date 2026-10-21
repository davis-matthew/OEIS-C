/*
 * A207925  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: DP over rows of a 5‑column binary array.
 * Horizontal rule: each row must avoid the 3‑bit patterns 000 and 010.
 * Vertical rule: in any column, the 3 consecutive bits of three
 * successive rows must not be 000 or 111.  Equivalently, if two
 * consecutive rows are equal in a column, the third row must be the
 * opposite bit in that column.
 * The DP state consists of the last two rows; from any state (i,j)
 * the admissible next rows k are pre‑computed in transMask[i][j].
 * The total number of arrays of height n is the sum over all states
 * after n−1 steps (for n≥3).  For n=1,2 the values are R and R²
 * respectively, where R is the number of admissible rows (15).
 */

#include <stdint.h>
#include <stdbool.h>

/* Max number of 5‑bit patterns that may have to be considered. */
#define MAXR 32

/* Static tables for caching the admissible rows. */
static int R = 0;                  /* number of admissible rows */
static uint32_t rows[MAXR];         /* admissible row patterns          */
static uint32_t transMask[MAXR][MAXR]; /* mask of admissible next rows    */
static bool initialized = false;   /* one‑time initialization flag     */

/* Return true if a 5‑bit pattern p obeys the horizontal restrictions. */
static bool is_allowed_row(uint32_t p)
{
    int i;
    for (i = 0; i < 3; ++i) {
        int a = (p >> i)       & 1;
        int b = (p >> (i + 1)) & 1;
        int c = (p >> (i + 2)) & 1;
        if (a == 0 && b == 0 && c == 0) return false;   /* 000 forbidden */
        if (a == 0 && b == 1 && c == 0) return false;   /* 010 forbidden */
    }
    return true;
}

/* One‑time setup: compute admissible rows and transition masks. */
static void init(void)
{
    int i, j, k, pos, idx = 0;

    /* Enumerate all admissible rows. */
    for (i = 0; i < 32; ++i) {
        if (is_allowed_row((uint32_t)i)) {
            rows[idx++] = (uint32_t)i;
        }
    }
    R = idx;

    /* Precompute transition masks. */
    for (i = 0; i < R; ++i) {
        for (j = 0; j < R; ++j) {
            uint32_t mask = 0;
            for (k = 0; k < R; ++k) {
                bool ok = true;
                for (pos = 0; pos < 5; ++pos) {
                    int b1 = (rows[i] >> pos) & 1;
                    int b2 = (rows[j] >> pos) & 1;
                    int b3 = (rows[k] >> pos) & 1;
                    if (b1 == b2 && b3 != (b1 ^ 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (1u << k);
            }
            transMask[i][j] = mask;
        }
    }

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1 */

    if (!initialized) init();

    /* N = 1: only one row, count admissible rows. */
    if (n == 1) {
        return (seq_t)R;
    }

    /* N = 2: any pair of admissible rows. */
    if (n == 2) {
        return (seq_t)R * (seq_t)R;
    }

    /* DP arrays: state (last two rows).  We use static arrays so
       the storage is valid across the function but we reinitialize
       them for each call. */
    static seq_t dpPrev[MAXR][MAXR];
    static seq_t dpCurr[MAXR][MAXR];
    int i, j, k;

    /* Initialize dpPrev to 1 for all pairs (height = 2). */
    for (i = 0; i < R; ++i) {
        for (j = 0; j < R; ++j) {
            dpPrev[i][j] = (seq_t)1;
        }
    }

    /* Run DP for heights 3 .. n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero dpCurr. */
        for (i = 0; i < R; ++i) {
            for (j = 0; j < R; ++j) {
                dpCurr[i][j] = 0;
            }
        }
        /* Transition from dpPrev to dpCurr. */
        for (i = 0; i < R; ++i) {
            for (j = 0; j < R; ++j) {
                seq_t val = dpPrev[i][j];
                if (val == 0) continue;
                uint32_t mask = transMask[i][j];
                for (k = 0; k < R; ++k) {
                    if (mask & (1u << k)) {
                        dpCurr[j][k] += val;
                    }
                }
            }
        }
        /* Swap dpPrev and dpCurr for next iteration. */
        for (i = 0; i < R; ++i) {
            for (j = 0; j < R; ++j) {
                dpPrev[i][j] = dpCurr[i][j];
            }
        }
    }

    /* Sum all entries of dpPrev to get the final count. */
    seq_t total = 0;
    for (i = 0; i < R; ++i) {
        for (j = 0; j < R; ++j) {
            total += dpPrev[i][j];
        }
    }
    return total;
}
