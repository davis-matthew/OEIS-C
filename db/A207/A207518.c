/*
 * A207518  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207518: Number of n×7 0/1 arrays avoiding
   horizontal 001 and 010 and vertical 010 and 101. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_COLS   7
#define MAX_ROWS   31
#define MAX_PAIRS  (MAX_ROWS * MAX_ROWS)

/* -- helpers -------------------------------------------------------- */
static bool horizontal_ok(uint8_t mask)
{
    for (int pos = 0; pos <= MAX_COLS - 3; ++pos) {
        int b0 = (mask >> pos) & 1;
        int b1 = (mask >> (pos + 1)) & 1;
        int b2 = (mask >> (pos + 2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 1) ||
            (b0 == 0 && b1 == 1 && b2 == 0))
            return false;
    }
    return true;
}

static bool vertical_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < MAX_COLS; ++j) {
        int x = (a >> j) & 1;
        int y = (b >> j) & 1;
        int z = (c >> j) & 1;
        if (x == 0 && y == 1 && z == 0) return false; /* 010 */
        if (x == 1 && y == 0 && z == 1) return false; /* 101 */
    }
    return true;
}

/* -- pre‑computation data ------------------------------------------ */
static uint8_t  rows[MAX_ROWS];
static int      next_idx[MAX_PAIRS][MAX_ROWS];
static signed char next_count[MAX_PAIRS];
static int      rows_count;
static int      pair_count;
static bool     initialized = false;

static void init(void)
{
    int rc = 0;
    for (int mask = 0; mask < (1 << MAX_COLS); ++mask)
        if (horizontal_ok((uint8_t)mask))
            rows[rc++] = (uint8_t)mask;
    rows_count = rc;           /* 31 */

    pair_count = rows_count * rows_count;   /* 961 */

    for (int a = 0; a < rows_count; ++a) {
        for (int b = 0; b < rows_count; ++b) {
            int p = a * rows_count + b;
            int ct = 0;
            for (int c = 0; c < rows_count; ++c)
                if (vertical_ok(rows[a], rows[b], rows[c]))
                    next_idx[p][ct++] = c;
            next_count[p] = (signed char)ct;
        }
    }
    initialized = true;
}

/* -- main sequence function ---------------------------------------- */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset 1 */

    if (!initialized) init();

    if (n == 1) return (seq_t)rows_count;
    if (n == 2) return (seq_t)rows_count * (seq_t)rows_count;

    size_t sz = (size_t)pair_count;

#ifdef __cplusplus
    std::vector<seq_t> cur(sz, (seq_t)1);
    std::vector<seq_t> next(sz, (seq_t)0);
#else
    seq_t *cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!cur || !next) { /* allocation failure */
        free(cur); free(next);
        return (seq_t)0;
    }
    for (size_t i = 0; i < sz; ++i) cur[i] = (seq_t)1;
#endif

    for (idx_t step = 3; step <= n; ++step) {
#ifdef __cplusplus
        std::fill(next.begin(), next.end(), (seq_t)0);
#else
        for (size_t i = 0; i < sz; ++i) next[i] = (seq_t)0;
#endif
        for (int p = 0; p < pair_count; ++p) {
            seq_t val = cur[p];
            if (val == (seq_t)0 || next_count[p] == 0) continue;
            int b  = p % rows_count;
            int ct = next_count[p];
            int *lst = next_idx[p];
            for (int k = 0; k < ct; ++k) {
                int c = lst[k];
                size_t idx = (size_t)b * rows_count + c;
                next[idx] += val;
            }
        }
#ifdef __cplusplus
        std::swap(cur, next);
#else
        seq_t *tmp = cur; cur = next; next = tmp;
#endif
    }

    seq_t total = (seq_t)0;
#ifdef __cplusplus
    for (const seq_t &v : cur) total += v;
#else
    for (size_t i = 0; i < sz; ++i) total += cur[i];
    free(cur); free(next);
#endif

    return total;
}
