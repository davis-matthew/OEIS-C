/*
 * A202488  Number of (n+2)X6 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*  A202488 – Number of (n+2)×6 binary arrays avoiding 000 and 010
 *  in every row, column and NW‑SE diagonal.
 *
 *  The function implements a dynamic‑programming algorithm over all
 *  6‑bit rows that themselves avoid the forbidden three‑bit patterns.
 *  For each n ≥ 1 it returns the exact count.  */

/* --------------------------------------------------------------------- */

static bool init_done = false;
static uint8_t allowedRows[64];
static uint8_t allowedCount;

/* --------------------------------------------------------------------- */
/*  Build the list of all 6‑bit rows that avoid 000 and 010 in any 3‑bit
 *  sliding window.  */
static void init_allowed_rows(void)
{
    allowedCount = 0;
    for (uint8_t mask = 0; mask < 64; ++mask) {
        bool ok = true;
        for (uint8_t shift = 0; shift <= 3; ++shift) {        /* windows 0..2,1..3,2..4,3..5 */
            uint8_t bits = (mask >> shift) & 7U;              /* 3‑bit window */
            if (bits == 0U || bits == 2U) {                  /* 000 or 010 */
                ok = false;
                break;
            }
        }
        if (ok) {
            allowedRows[allowedCount++] = mask;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Check that three rows (a,b,c) satisfy the 000/010 constraints in
 *  every column and every NW→SE diagonal. */
static bool valid_transition(uint8_t a, uint8_t b, uint8_t c)
{
    /* vertical triples – columns 0..5 */
    for (uint8_t col = 0; col < 6; ++col) {
        uint8_t top    = (a >> col) & 1U;
        uint8_t middle = (b >> col) & 1U;
        uint8_t bot    = (c >> col) & 1U;
        if ((top   == 0U && middle == 0U && bot == 0U) ||
            (top   == 0U && middle == 1U && bot == 0U))
            return false;
    }
    /* NW‑SE diagonal triples – positions (b-1,b,c) for columns 2..5 */
    for (uint8_t col = 2; col < 6; ++col) {
        uint8_t top    = (a >> (col - 2)) & 1U;
        uint8_t middle = (b >> (col - 1)) & 1U;
        uint8_t bot    = (c >> col) & 1U;
        if ((top   == 0U && middle == 0U && bot == 0U) ||
            (top   == 0U && middle == 1U && bot == 0U))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  The required OEIS sequence generator.  */
seq_t a(idx_t n)
{
    /* Offset 1 – undefined for n < 1 */
    if (n < 1)
        return (seq_t)0;

    if (!init_done) {
        init_allowed_rows();
        init_done = true;
    }

    const idx_t rows = n + 2U;          /* total rows of the array */
    if (rows < 3U)                      /* defensive */
        return (seq_t)0;

    /* DP tables: cur[i][j] counts arrays ending with
     *      prev_second = allowedRows[i]
     *      prev_last   = allowedRows[j]   */
    static seq_t cur[64][64];
    static seq_t nxt[64][64];
    seq_t (*pcur)[64] = cur;
    seq_t (*pnxt)[64] = nxt;

    /* initialise tables */
    memset(cur, 0, sizeof(cur));
    memset(nxt, 0, sizeof(nxt));

    for (uint8_t i = 0; i < allowedCount; ++i)
        for (uint8_t j = 0; j < allowedCount; ++j)
            pcur[i][j] = (seq_t)1;   /* any pair of allowed rows */

    /* Add one row at a time until we reach the required height */
    for (idx_t step = 0; step < n; ++step) {
        memset(pnxt[0], 0, sizeof(seq_t) * 64 * 64);

        for (uint8_t i = 0; i < allowedCount; ++i) {          /* prev_last   */
            for (uint8_t j = 0; j < allowedCount; ++j) {      /* prev_second */
                seq_t val = pcur[i][j];
                if (val == (seq_t)0)
                    continue;
                uint8_t prev_last   = allowedRows[i];
                uint8_t prev_second = allowedRows[j];
                for (uint8_t k = 0; k < allowedCount; ++k) {  /* candidate row */
                    uint8_t next_row = allowedRows[k];
                    if (!valid_transition(prev_second, prev_last, next_row))
                        continue;
                    /* new state: (prev_last, next_row) → indices (i,k) */
                    pnxt[k][i] += val;
                }
            }
        }

        /* swap current and next tables */
        seq_t (*tmp)[64] = pcur;
        pcur = pnxt;
        pnxt = tmp;
    }

    /* Sum over all final states */
    seq_t total = (seq_t)0;
    for (uint8_t i = 0; i < allowedCount; ++i)
        for (uint8_t j = 0; j < allowedCount; ++j)
            total += pcur[i][j];

    return total;
}
