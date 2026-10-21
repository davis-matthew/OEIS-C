/*
 * A237215  Number of (n+1)X(4+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by transfer‑matrix DP on 5‑column rows */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* 5‑column row space : 4‑ary digits, 4^5 = 1024 possible rows */
#define ROWS 1024
#define MAX_E 7   /* possible values of the block expression, 0..6 */

/* Global tables constructed once */
static unsigned char ROW[ROWS][5];                 /* row encoding */
static uint16_t  ADJ[MAX_E][ROWS][ROWS];          /* adjacency lists (fixed size) */
static uint16_t  ADJL[MAX_E][ROWS];               /* number of neighbours per row */
static int       INIT = 0;

/* Build the global tables */
static void build_tables(void)
{
    /* encode all rows */
    for (int i = 0; i < ROWS; ++i) {
        int v = i;
        for (int k = 0; k < 5; ++k) {
            ROW[i][k] = v & 3;
            v >>= 2;
        }
    }

    /* pre‑allocate adjacency tables with zeros (global static, already zero) */

    /* compute adjacency for each ordered pair of rows */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        const unsigned char *a = ROW[r1];
        for (int r2 = 0; r2 < ROWS; ++r2) {
            const unsigned char *b = ROW[r2];
            int e = -1;
            /* examine 4 column pairs */
            for (int c = 0; c < 4; ++c) {
                unsigned char arr[4] = { a[c], a[c+1], b[c], b[c+1] };
                /* simple 4‑element sort */
                for (int p = 0; p < 3; ++p)
                    for (int q = p + 1; q < 4; ++q)
                        if (arr[q] < arr[p]) {
                            unsigned char t = arr[p];
                            arr[p] = arr[q];
                            arr[q] = t;
                        }
                int val = arr[3] + arr[2] - arr[1] - arr[0];
                if (e == -1) e = val;
                else if (e != val) { e = -1; break; }
            }
            if (e >= 0) {
                uint16_t *list = ADJ[e][r1];
                uint16_t len = ADJL[e][r1];
                list[len++] = (uint16_t)r2;
                ADJL[e][r1] = len;
            }
        }
    }
    INIT = 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise global tables on first call */
    if (!INIT) build_tables();

    seq_t result = (seq_t)0;
    /* temporary DP arrays */
    static seq_t dp[ROWS];
    static seq_t next[ROWS];

    /* loop over all possible constant expressions e */
    for (int e = 0; e < MAX_E; ++e) {
        /* start with one way to end at each row (length 1) */
        for (int i = 0; i < ROWS; ++i) dp[i] = (seq_t)1;

        /* n transitions → n+1 rows total */
        for (idx_t step = 0; step < n; ++step) {
            /* clear next */
            for (int i = 0; i < ROWS; ++i) next[i] = (seq_t)0;

            /* transition step */
            for (int i = 0; i < ROWS; ++i) {
                seq_t cur = dp[i];
                uint16_t len = ADJL[e][i];
                const uint16_t *list = ADJ[e][i];
                for (uint16_t k = 0; k < len; ++k) {
                    int j = list[k];
                    next[j] += cur;
                }
            }

            /* copy next to dp for next iteration */
            for (int i = 0; i < ROWS; ++i) dp[i] = next[i];
        }

        /* sum all ending states for this e */
        for (int i = 0; i < ROWS; ++i) result += dp[i];
    }

    return result;
}
