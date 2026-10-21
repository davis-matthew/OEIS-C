/*
 * A207567  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding * horizontal
 *            patterns 0 0 1 and 0 1 0 and vertical patterns 0 1 1 and 1 1 0.
 *            * * Method: Dynamic programming over column transitions. * 1.
 *            Enumerate all 6‑bit columns that satisfy the vertical
 *            constraints * (no 011 or 110 within the column). Let m be th...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define MAX_BITS 6
#define MAX_VALS (1 << MAX_BITS)         /* 64 possible 6‑bit columns */
#define STATE_MAX (MAX_VALS * MAX_VALS)   /* 4096 states */

/* ---------- tables for vertical‑valid columns and transitions ---------- */
static int         validCols[MAX_VALS];
static int         valCount = 0;            /* number of valid columns */

static int         allowedNext[MAX_VALS][MAX_VALS][MAX_VALS];
static int         allowedCnt [MAX_VALS][MAX_VALS];

static int         initialized = 0;          /* flag for table creation */

/* ---------- helper: check vertical pattern on a 6‑bit column ---------- */
static int vertical_ok(int v)
{
    for (int r = 0; r <= MAX_BITS - 3; ++r) {
        int a = (v >> r) & 1;
        int b = (v >> (r + 1)) & 1;
        int c = (v >> (r + 2)) & 1;
        if ((a == 0 && b == 1 && c == 1) ||
            (a == 1 && b == 1 && c == 0))
            return 0;
    }
    return 1;
}

/* ---------- initialise tables once ----------------------------------- */
static void init_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* collect all vertically valid columns */
    for (int v = 0; v < MAX_VALS; ++v) {
        if (vertical_ok(v))
            validCols[valCount++] = v;
    }

    /* pre‑compute allowed third columns for each ordered pair (x,y) */
    for (int ix = 0; ix < valCount; ++ix) {
        int x = validCols[ix];
        for (int iy = 0; iy < valCount; ++iy) {
            int y = validCols[iy];
            int cnt = 0;
            for (int iz = 0; iz < valCount; ++iz) {
                int z = validCols[iz];
                int ok = 1;
                for (int r = 0; r < MAX_BITS; ++r) {
                    int a = (x >> r) & 1;
                    int b = (y >> r) & 1;
                    int c = (z >> r) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 0 && b == 1 && c == 0)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    allowedNext[ix][iy][cnt++] = iz;
                }
            }
            allowedCnt[ix][iy] = cnt;
        }
    }
}

/* ---------- DP storage (static to avoid dynamic allocation) ---------- */
static seq_t dpA[STATE_MAX];
static seq_t dpB[STATE_MAX];

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    init_tables();                       /* build tables on first use */

    /* base cases */
    if (n == 1) return (seq_t)valCount;
    if (n == 2) return (seq_t)((seq_t)valCount * (seq_t)valCount);

    /* initialise dp for length 2: all pairs have count 1 */
    for (int i = 0; i < valCount * valCount; ++i) dpA[i] = (seq_t)1;

    seq_t total = 0;                     /* a(n) will be stored here */

    /* iterate from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero next DP array */
        for (int i = 0; i < valCount * valCount; ++i) dpB[i] = (seq_t)0;

        /* perform transitions */
        for (int ix = 0; ix < valCount; ++ix) {
            for (int iy = 0; iy < valCount; ++iy) {
                seq_t curCnt = dpA[ix * valCount + iy];
                if (curCnt == (seq_t)0) continue;

                int cnt = allowedCnt[ix][iy];
                for (int t = 0; t < cnt; ++t) {
                    int iz = allowedNext[ix][iy][t];
                    dpB[iy * valCount + iz] += curCnt;
                }
            }
        }

        /* compute a(len) as sum over all states */
        total = (seq_t)0;
        for (int i = 0; i < valCount * valCount; ++i) {
            total += dpB[i];
        }

        /* prepare for next iteration: swap dpA and dpB */
        for (int i = 0; i < valCount * valCount; ++i) {
            dpA[i] = dpB[i];
        }
    }

    return total;      /* a(n) after the loop */
}
