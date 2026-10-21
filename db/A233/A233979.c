/*
 * A233979  Number of (n+1)X(6+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 13.
 * https://oeis.org/A233979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays of 0..6 where every 2×2 block
 *            satisfies that the sum of absolute values of the six edge and
 *            diagonal differences equals 13. This implementation uses a
 *            transfer‑matrix over all possible rows. The number of rows is
 *            7^7=823543. For each row we pre–compute all admissib...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   is_abs_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_abs_diff.h"

/* pre–computation of allowed (c,d) for each top pair (a,b) */
#define MAX_ALLOWED 10
static uint8_t allowC[49][MAX_ALLOWED];
static uint8_t allowD[49][MAX_ALLOWED];
static int allowCnt[49];

/* all rows: 823543 rows × 7 digits */
#define ROWS 823543
static uint8_t rows[ROWS][7];

/* adjacency data */
static int *adj_start;     /* start index in edgesAll for each row   */
static int *adj_deg;       /* number of outgoing edges per row */
static int *edgesAll;      /* contiguous list of all target rows  */
static size_t edgesAllLen;
static size_t edgesAllCap;

/* DP data */
static seq_t *dp_cur;
static seq_t *dp_next;
static size_t cur_n;          /* number of steps already processed */
static seq_t *ansCache;       /* cached answers for 0..max_n */
static bool *ansKnown;
static size_t ansCacheSize;

/* flag whether initialisation has been done */
static bool initialised = false;

/* encode a row as base‑7 integer (0 .. 823542) */
static inline int encode_row(const uint8_t *row)
{
    int code = 0;
    for (int i = 0; i < 7; ++i) code = code * 7 + row[i];
    return code;
}

/* recursive generator of all successor rows for a given row */
static void dfs_gen(int pos, const uint8_t *row, uint8_t *next, int *out_deg)
{
    if (pos == 6) {
        int code = encode_row(next);
        if (edgesAllLen == edgesAllCap) {
            edgesAllCap <<= 1;
            edgesAll = (int *)realloc(edgesAll, edgesAllCap * sizeof(int));
        }
        edgesAll[edgesAllLen++] = code;
        ++(*out_deg);
        return;
    }

    if (pos == 0) {
        int pair = row[0] * 7 + row[1];
        for (int k = 0; k < allowCnt[pair]; ++k) {
            next[0] = allowC[pair][k];
            next[1] = allowD[pair][k];
            dfs_gen(1, row, next, out_deg);
        }
    } else {
        uint8_t left = next[pos];
        int pair = row[pos] * 7 + row[pos + 1];
        for (int k = 0; k < allowCnt[pair]; ++k) {
            if (allowC[pair][k] == left) {
                next[pos + 1] = allowD[pair][k];
                dfs_gen(pos + 1, row, next, out_deg);
            }
        }
    }
}

/* perform one DP step */
static void dp_step(void)
{
    /* zero dp_next */
    for (size_t i = 0; i < ROWS; ++i) dp_next[i] = 0;

    /* transition */
    for (size_t i = 0; i < ROWS; ++i) {
        seq_t val = dp_cur[i];
        if (val == 0) continue;
        int start = adj_start[i];
        int deg   = adj_deg[i];
        for (int k = 0; k < deg; ++k) {
            int j = edgesAll[start + k];
            dp_next[j] += val;
        }
    }
    /* compute sum and store answer */
    seq_t total = 0;
    for (size_t i = 0; i < ROWS; ++i) total += dp_next[i];
    ++cur_n;
    if (cur_n >= ansCacheSize) {
        size_t newSize = cur_n + 1;
        ansCache = (seq_t *)realloc(ansCache, newSize * sizeof(seq_t));
        ansKnown = (bool *)realloc(ansKnown, newSize * sizeof(bool));
        for (size_t k = ansCacheSize; k < newSize; ++k) ansKnown[k] = false;
        ansCacheSize = newSize;
    }
    ansCache[cur_n] = total;
    ansKnown[cur_n] = true;

    /* swap dp_cur and dp_next for next iteration */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;
}

/* initialisation routine */
static void init(void)
{
    /* compute allowed pairs */
    for (int a = 0; a <= 6; ++a)
        for (int b = 0; b <= 6; ++b) {
            int idx = a * 7 + b;
            allowCnt[idx] = 0;
            for (int c = 0; c <= 6; ++c)
                for (int d = 0; d <= 6; ++d) {
                    int sum =
                        is_abs_diff(a, b) + is_abs_diff(a, c) + is_abs_diff(c, d) +
                        is_abs_diff(b, d) + is_abs_diff(a, d) + is_abs_diff(b, c);
                    if (sum == 13) {
                        allowC[idx][allowCnt[idx]] = (uint8_t)c;
                        allowD[idx][allowCnt[idx]] = (uint8_t)d;
                        ++allowCnt[idx];
                    }
                }
        }

    /* enumerate all rows and fill rows[][] */
    for (int code = 0; code < ROWS; ++code) {
        int tmp = code;
        for (int pos = 6; pos >= 0; --pos) {
            rows[code][pos] = tmp % 7;
            tmp /= 7;
        }
    }

    /* allocate adjacency containers */
    adj_start = (int *)malloc(ROWS * sizeof(int));
    adj_deg   = (int *)malloc(ROWS * sizeof(int));
    edgesAllCap = ROWS * 4;                   /* initial guess */
    edgesAll = (int *)malloc(edgesAllCap * sizeof(int));
    edgesAllLen = 0;

    /* build adjacency list */
    uint8_t next[7];
    for (int idx = 0; idx < ROWS; ++idx) {
        adj_start[idx] = (int)edgesAllLen;
        int deg = 0;
        dfs_gen(0, rows[idx], next, &deg);
        adj_deg[idx] = deg;
    }

    /* DP buffers */
    dp_cur = (seq_t *)malloc(ROWS * sizeof(seq_t));
    dp_next = (seq_t *)malloc(ROWS * sizeof(seq_t));
    for (size_t i = 0; i < ROWS; ++i) dp_cur[i] = 1;   /* initial state */
    cur_n = 0;

    /* answer cache */
    ansCacheSize = 1;
    ansCache = (seq_t *)malloc(ansCacheSize * sizeof(seq_t));
    ansKnown = (bool *)malloc(ansCacheSize * sizeof(bool));
    ansCache[0] = 0;        /* unused */
    ansKnown[0] = true;

    initialised = true;
}

/* public function */
seq_t a(idx_t n)
{
    if (!initialised) init();

    if (n >= ansCacheSize) {
        size_t newSize = n + 1;
        ansCache = (seq_t *)realloc(ansCache, newSize * sizeof(seq_t));
        ansKnown = (bool *)realloc(ansKnown, newSize * sizeof(bool));
        for (size_t k = ansCacheSize; k < newSize; ++k) ansKnown[k] = false;
        ansCacheSize = newSize;
    }

    if (!ansKnown[n]) {
        while (cur_n < n) dp_step();
    }
    return ansCache[n];
}
