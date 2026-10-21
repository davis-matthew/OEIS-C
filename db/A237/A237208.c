/*
 * A237208  Number of (n+1)X(5+1) 0..3 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transfer‑matrix implementation for OEIS A237208
   The implementation constructs the adjacency graph of rows
   according to the constant 2×2 block condition and counts row
   sequences by dynamic programming.  The order n corresponds to
   a (n+1)×6 array; the number of transitions equals n.  */
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
 
/* Constants */
#define NUM_ROWS 4096          /* 4^6  rows */
#define NUM_COLS 6
#define MAX_K    7             /* possible values of K = 0..6 */

/* Local static data (created once) */
static uint8_t val[NUM_ROWS][NUM_COLS];    /* decoded rows */
static unsigned char Ktab[4][4][4][4];     /* K value for a pair of 2x2 blocks */
static size_t row_start[MAX_K][NUM_ROWS+1];/* prefix sums of edges per row */
static uint16_t *edges[MAX_K];             /* adjacency lists (flattened) */
static int init_done = 0;                  /* initialization flag */

/* ------------------------------------------------------------ */
/* utility to compute K for a 2×2 block (a,b,c,d) */
static unsigned char compute_K(unsigned char a, unsigned char b,
                               unsigned char c, unsigned char d)
{
    unsigned char arr[4] = { a, b, c, d };
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (arr[i] > arr[j]) {
                unsigned char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
    return arr[1] + arr[2] - arr[0];
}

/* ------------------------------------------------------------ */
static void build_transfers(void)
{
    /* --- decode all rows into six digits of base 4 ---------------- */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int t = r;
        for (int k = 0; k < NUM_COLS; ++k) {
            val[r][k] = t & 3;
            t >>= 2;
        }
    }

    /* --- precompute Ktable ----------------------------------------- */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d)
                    Ktab[a][b][c][d] = compute_K(a, b, c, d);

    /* --- first pass: count edges --------------------------------- */
    static int cnt[MAX_K][NUM_ROWS];
    memset(cnt, 0, sizeof(cnt));

    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            int valid = 1;
            unsigned char k_val = 0;
            for (int j = 0; j < NUM_COLS - 1; ++j) {
                unsigned char kj = Ktab[val[r1][j]][val[r1][j+1]]
                                  [val[r2][j]][val[r2][j+1]];
                if (j == 0) k_val = kj;
                else if (kj != k_val) { valid = 0; break; }
            }
            if (valid) cnt[k_val][r1] += 1;
        }
    }

    /* --- compute prefix sums ------------------------------------- */
    for (int k = 0; k < MAX_K; ++k) {
        row_start[k][0] = 0;
        for (int i = 0; i < NUM_ROWS; ++i)
            row_start[k][i+1] = row_start[k][i] + cnt[k][i];
    }

    /* --- allocate adjacency buffers ------------------------------- */
    for (int k = 0; k < MAX_K; ++k) {
        size_t total = row_start[k][NUM_ROWS];
        edges[k] = (uint16_t*)malloc(total * sizeof(uint16_t));
    }

    /* --- second pass: fill adjacency ----------------------------- */
    static size_t offs[MAX_K][NUM_ROWS];
    for (int k = 0; k < MAX_K; ++k)
        for (int i = 0; i < NUM_ROWS; ++i)
            offs[k][i] = row_start[k][i];

    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            int valid = 1;
            unsigned char k_val = 0;
            for (int j = 0; j < NUM_COLS - 1; ++j) {
                unsigned char kj = Ktab[val[r1][j]][val[r1][j+1]]
                                  [val[r2][j]][val[r2][j+1]];
                if (j == 0) k_val = kj;
                else if (kj != k_val) { valid = 0; break; }
            }
            if (valid) {
                size_t idx = offs[k_val][r1]++;
                edges[k_val][idx] = (uint16_t)r2;
            }
        }
    }

    init_done = 1;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) build_transfers();

    /* DP arrays: use two rows of seq_t */
    seq_t *dp  = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));
    seq_t *nex = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));
    seq_t result = 0;

    size_t steps = (size_t)n;   /* number of transitions = n */

    for (int k = 0; k < MAX_K; ++k) {
        /* initialize dp to 1 for all starting rows */
        for (size_t i = 0; i < NUM_ROWS; ++i) dp[i] = 1;

        for (size_t step = 0; step < steps; ++step) {
            /* zero nex */
            memset(nex, 0, NUM_ROWS * sizeof(seq_t));

            for (size_t i = 0; i < NUM_ROWS; ++i) {
                seq_t val = dp[i];
                if (val == 0) continue;
                size_t start = row_start[k][i];
                size_t end   = row_start[k][i+1];
                for (size_t e = start; e < end; ++e) {
                    uint16_t j = edges[k][e];
                    nex[j] += val;
                }
            }
            /* swap dp and nex */
            seq_t *tmp = dp;
            dp = nex;
            nex = tmp;
        }

        /* sum all dp entries into result */
        for (size_t i = 0; i < NUM_ROWS; ++i)
            result += dp[i];
    }

    free(dp);
    free(nex);
    return result;
}
