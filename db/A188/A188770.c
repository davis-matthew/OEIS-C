/*
 * A188770  Number of nX5 binary arrays without the pattern 0 1 0 vertically
 *          or horizontally.
 * https://oeis.org/A188770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188770: Number of n×5 binary arrays without continuous 010 vertically or horizontally.
   The function computes the term by dynamic programming over the last two rows.
   a(1) = 21, a(2) = 441, a(3) = 5357, … (start of the OEIS list). */

#include <stdbool.h>
#include <stdint.h>

enum { MAXM = 32 };            /* sufficient for 21 valid rows */

/* Global tables, filled once in init() */
static bool initialized = false;
static unsigned M;                     /* number of valid rows (21) */
static uint8_t rows[MAXM];             /* row bit patterns 0..31 */
static uint8_t transCount[MAXM][MAXM]; /* counts of successors */
static int transList[MAXM][MAXM][MAXM];/* successors indices */

/* Test whether a 5‑bit row contains the horizontal pattern 010. */
static inline bool ok_row(uint8_t r)
{
    for (int i = 0; i < 3; ++i) {
        if (((r >> i) & 7U) == 0x2U) return false; /* bits i,i+1,i+2 == 010 */
    }
    return true;
}

/* Test whether a triple of rows (a,b,c) contains the vertical pattern 010
   in any column. */
static inline bool ok_triple(uint8_t a, uint8_t b, uint8_t c)
{
    for (int col = 0; col < 5; ++col) {
        int ba = (a >> col) & 1;
        int bb = (b >> col) & 1;
        int bc = (c >> col) & 1;
        if (ba == 0 && bb == 1 && bc == 0) return false;
    }
    return true;
}

/* One‑time initialization of global tables. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* 1. Enumerate all 5‑bit rows without horizontal 010. */
    M = 0;
    for (uint8_t r = 0; r < 32; ++r) {
        if (ok_row(r)) rows[M++] = r;
    }

    /* 2. For each ordered pair (i,j) compute all legal successors k. */
    for (int i = 0; i < (int)M; ++i) {
        for (int j = 0; j < (int)M; ++j) {
            unsigned cnt = 0;
            for (int k = 0; k < (int)M; ++k) {
                if (ok_triple(rows[i], rows[j], rows[k])) {
                    transList[i][j][cnt++] = k;
                }
            }
            transCount[i][j] = cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();

    /* Special cases */
    if (n == 1ULL) return (seq_t)M;                  /* single row: any valid row */
    if (n == 2ULL) return (seq_t)M * (seq_t)M;       /* two rows: any pair is fine */

    /* Dynamic programming over the last two rows.
       dp_cur[i][j] – number of ways ending with rows[i], rows[j]
       dp_next[b][c] – number of ways after adding a new row c. */
    seq_t dp_cur[MAXM][MAXM];
    seq_t dp_next[MAXM][MAXM];

    /* Initialise for two rows: exactly one way for each pair (i,j) */
    for (int i = 0; i < (int)M; ++i) {
        for (int j = 0; j < (int)M; ++j) {
            dp_cur[i][j] = (seq_t)1;
        }
    }

    /* Add rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < (int)M; ++i)
            for (int j = 0; j < (int)M; ++j)
                dp_next[i][j] = (seq_t)0;

        /* transition */
        for (int a = 0; a < (int)M; ++a) {
            for (int b = 0; b < (int)M; ++b) {
                seq_t ways = dp_cur[a][b];
                if (!ways) continue;
                unsigned cnt = transCount[a][b];
                for (unsigned t = 0; t < cnt; ++t) {
                    int c = transList[a][b][t];
                    dp_next[b][c] += ways;
                }
            }
        }

        /* swap dp_cur and dp_next for the next iteration */
        for (int i = 0; i < (int)M; ++i)
            for (int j = 0; j < (int)M; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Sum all possibilities for the last two rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < (int)M; ++i)
        for (int j = 0; j < (int)M; ++j)
            total += dp_cur[i][j];

    return total;
}
