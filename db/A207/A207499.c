/*
 * A207499  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding * horizontal 000 and
 *            011, and vertical 001 and 100 in any * column of 3 consecutive
 *            rows. * * Implementation uses a transfer‑matrix dynamic
 *            programming * over pairs of consecutive rows. Only 25 of the 2^7
 *            * row patterns satisfy the horizontal constraint...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool initialized = false;
static int  Vcnt = 0;                 /* number of valid rows (25)          */
static uint8_t  Vrows[128];           /* pattern of each valid row          */
static int      idxRow[128];          /* map pattern -> index (-1 if none)  */
static seq_t f_cur[128][128], f_next[128][128];  /* DP tables */

/* Check that a 7‑bit row avoids 000 and 011. */
static bool valid_row(uint8_t r)
{
    for (int k = 0; k <= 4; ++k) {
        uint8_t bits = (r >> k) & 0x7;                 /* three consecutive bits */
        if (bits == 0x0 || bits == 0x3) return false; /* 000 or 011 */
    }
    return true;
}

/* Check that the 3‑row triple (a,b,c) avoids the vertical patterns
 * 0 0 1 and 1 0 0 in every column. */
static bool vertical_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int k = 0; k < 7; ++k) {
        uint8_t x = (a >> k) & 1;
        uint8_t y = (b >> k) & 1;
        uint8_t z = (c >> k) & 1;
        if (x == 0 && y == 0 && z == 1) return false;
        if (x == 1 && y == 0 && z == 0) return false;
    }
    return true;
}

/* One‑time initialization: enumerate valid rows and build maps. */
static void init(void)
{
    for (int i = 0; i < 128; ++i) idxRow[i] = -1;

    Vcnt = 0;
    for (int r = 0; r < 128; ++r) {
        if (valid_row((uint8_t)r)) {
            Vrows[Vcnt] = (uint8_t)r;
            idxRow[r] = Vcnt;
            ++Vcnt;
        }
    }

    /* DP tables already zero‑initialized by static storage */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return (seq_t)0;          /* out of offset, arbitrary */
    if (n == 1) return (seq_t)Vcnt;        /* single row */
    if (n == 2) return (seq_t)Vcnt * Vcnt;

    /* Initialise f_cur[a][b] = 1 for all pairs of valid rows. */
    for (int i = 0; i < Vcnt; ++i)
        for (int j = 0; j < Vcnt; ++j)
            f_cur[i][j] = (seq_t)1;

    /* DP for rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero f_next */
        for (int i = 0; i < Vcnt; ++i)
            for (int j = 0; j < Vcnt; ++j)
                f_next[i][j] = (seq_t)0;

        for (int a = 0; a < Vcnt; ++a) {
            for (int b = 0; b < Vcnt; ++b) {
                seq_t val = f_cur[a][b];
                if (val == (seq_t)0) continue;
                uint8_t ra = Vrows[a], rb = Vrows[b];
                for (int c = 0; c < Vcnt; ++c) {
                    if (!vertical_ok(ra, rb, Vrows[c])) continue;
                    f_next[b][c] += val;
                }
            }
        }

        /* swap f_cur and f_next */
        for (int i = 0; i < Vcnt; ++i)
            for (int j = 0; j < Vcnt; ++j)
                f_cur[i][j] = f_next[i][j];
    }

    /* Sum over all ending pairs to get final answer */
    seq_t total = (seq_t)0;
    for (int i = 0; i < Vcnt; ++i)
        for (int j = 0; j < Vcnt; ++j)
            total += f_cur[i][j];

    return total;
}
