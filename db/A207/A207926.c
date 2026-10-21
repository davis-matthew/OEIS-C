/*
 * A207926  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   is_valid_horizontal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A207926: Count n×6 binary arrays avoiding horizontal 000 and 010,
 * and vertical 000 and 111 in any 3 consecutive rows.
 * The algorithm builds the set of admissible 6‑bit rows, precomputes
 * legal transitions for consecutive rows, then performs a dynamic
 * programming over pairs of rows.  The DP table has size M², where
 * M is the number of admissible rows (M=25 for 6 columns).  The
 * answer for n≥3 is obtained after n‑2 steps of the DP.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_valid_horizontal.h"

static bool vertical_ok(uint8_t a, uint8_t b, uint8_t c)
{
    /* for each column the 3 bits must not be all 0 or all 1 */
    for (int j = 0; j < 6; ++j) {
        uint8_t bits =
            ((a >> j) & 1) |
            (((b >> j) & 1) << 1) |
            (((c >> j) & 1) << 2);
        if (bits == 0 || bits == 7)
            return false;                    /* 000 or 111 */
    }
    return true;
}

/* ---------- state tables ---------- */
static unsigned char validMasks[64];
static int M = 0;                                  /* number of valid rows */

/* for each pair (i,j) list of indices k that can follow */
struct transEntry {
    unsigned char *list;                          /* list of k indices */
    unsigned char count;                          /* list length */
};
static struct transEntry trans[64][64];

static seq_t *dpPrev = NULL;                      /* dp for rows r-2,r-1 */
static seq_t *dpCurr = NULL;                      /* dp for rows r-1,r   */
static bool initialized = false;

/* ---------- lazy initialization ---------- */
static void init(void)
{
    if (initialized)
        return;

    /* 1. collect all valid rows */
    M = 0;
    for (uint8_t m = 0; m < 64; ++m)
        if (is_valid_horizontal(m))
            validMasks[M++] = m;

    /* 2. allocate DP arrays */
    size_t S = (size_t)M * (size_t)M;
    dpPrev = (seq_t *)calloc(S, sizeof(seq_t));
    dpCurr = (seq_t *)calloc(S, sizeof(seq_t));

    /* 3. pre‑compute transitions */
    unsigned char temp[64];
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            unsigned char cnt = 0;
            for (int k = 0; k < M; ++k) {
                if (vertical_ok(validMasks[i], validMasks[j], validMasks[k]))
                    temp[cnt++] = (unsigned char)k;
            }
            trans[i][j].count = cnt;
            if (cnt > 0) {
                trans[i][j].list = (unsigned char *)malloc(cnt * sizeof(unsigned char));
                memcpy(trans[i][j].list, temp, cnt * sizeof(unsigned char));
            } else {
                trans[i][j].list = NULL;
            }
        }
    }
    initialized = true;
}

/* ---------- public API ---------- */
seq_t a(idx_t n)
{
    if (n == 0)     /* below offset, not relevant */
        return 0;
    init();

    if (n == 1)         /* single row: any valid mask is allowed */
        return (seq_t)M;

    if (n == 2) {       /* two independent rows: all pairs allowed */
        seq_t tmp = (seq_t)M;
        return tmp * tmp;
    }

    /* DP: start with all pairs having count 1 (row 1 and 2) */
    size_t S = (size_t)M * (size_t)M;
    for (size_t idx = 0; idx < S; ++idx)
        dpPrev[idx] = 1;

    /* iterate from row 3 up to n */
    for (idx_t row = 3; row <= n; ++row) {
        for (size_t idx = 0; idx < S; ++idx)
            dpCurr[idx] = 0;

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t val = dpPrev[(size_t)i * M + j];
                if (!val)
                    continue;
                const unsigned char *list = trans[i][j].list;
                unsigned char cnt = trans[i][j].count;
                if (!cnt)
                    continue;
                for (unsigned char t = 0; t < cnt; ++t) {
                    int k = list[t];
                    dpCurr[(size_t)j * M + k] += val;
                }
            }
        }
        /* swap dpPrev / dpCurr for next iteration */
        seq_t *tmp = dpPrev;
        dpPrev = dpCurr;
        dpCurr = tmp;
    }

    /* sum over all final states */
    seq_t total = 0;
    for (size_t idx = 0; idx < S; ++idx)
        total += dpPrev[idx];

    return total;
}
