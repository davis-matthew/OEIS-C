/*
 * A206203  Number of (n+1)X6 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 18, u64 n <= 47, u128 n <= 105,
 *            big any n
 * Helpers:   clockwise_inc_2x3, clockwise_inc_3x2, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A206203
 *  a(n) = 10*a(n-3)  for n>15
 *  The first 15 terms are computed by dynamic programming.
 *  The function returns the exact value for any n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */
#include "../utils/seq_mul.h" /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/clockwise_inc_2x3.h"
#include "../utils/clockwise_inc_3x2.h"

#define ROWS 4096              /* 4^6 possible rows of length 6 */
#define COLS 6
#define SHIFT 12               /* 1<<12 == 4096 */

typedef struct {
    uint32_t pair;   /* (p<<SHIFT | q)  12+12 bits */
    uint64_t count;  /* number of partial arrays having this state */
} state_t;

/* row_digits[p][c] gives the cth digit of row p (0‑based)   */
static uint8_t row_digits[ROWS][COLS];

/* good2x3[p*ROWS+q] == 1 if rows p,q form a valid 2×3 block */
static uint8_t *good2x3 = NULL;

/* Base values a(1)..a(15)  (index 1..15) */
static seq_t values[16];

/* Flag that indicates that the initial terms have been computed. */
static bool initialized = false;

static int compare_state(const void *a, const void *b)
{
    const state_t *lhs = (const state_t *)a;
    const state_t *rhs = (const state_t *)b;
    if (lhs->pair < rhs->pair) return -1;
    if (lhs->pair > rhs->pair) return 1;
    return 0;
}

/* valid 2×3 pair */
static inline bool valid_2x3_pair(int p, int q)
{
    const uint8_t *r0 = row_digits[p];
    const uint8_t *r1 = row_digits[q];
    for (int col = 0; col < 4; ++col)
        if (clockwise_inc_2x3(r0, r1, col) != 1)
            return false;
    return true;
}

/* valid 3×2 triple */
static inline bool valid_3x2_triple(int p, int q, int r)
{
    const uint8_t *r0 = row_digits[p];
    const uint8_t *r1 = row_digits[q];
    const uint8_t *r2 = row_digits[r];
    for (int j = 0; j < 5; ++j)
        if (clockwise_inc_3x2(r0, r1, r2, j) != 1)
            return false;
    return true;
}

/* compute the first 15 terms once */
static void compute_initial_terms(void)
{
    if (initialized) return;
    initialized = true;

    /* Encode all rows (0..4095) as 6 digits base‑4 */
    for (int pat = 0; pat < ROWS; ++pat) {
        for (int k = 0; k < COLS; ++k)
            row_digits[pat][k] = (pat >> (2 * k)) & 3;
    }

    /* Allocate table for good 2×3 pairs */
    good2x3 = (uint8_t *)malloc(ROWS * ROWS);
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q)
            good2x3[p * ROWS + q] = valid_2x3_pair(p, q);

    /* Initial states: all valid 2×3 pairs (first two rows). */
    size_t cur_cap = 128, cur_len = 0;
    state_t *cur = (state_t *)malloc(cur_cap * sizeof(state_t));

    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            if (!good2x3[p * ROWS + q]) continue;
            if (cur_len == cur_cap) {
                cur_cap <<= 1;
                cur = (state_t *)realloc(cur, cur_cap * sizeof(state_t));
            }
            cur[cur_len].pair = (p << SHIFT) | q;
            cur[cur_len].count = 1;
            ++cur_len;
        }
    }

    /* Sum counts for n = 1 */
    seq_t sum = 0;
    for (size_t i = 0; i < cur_len; ++i)
        sum += (seq_t)cur[i].count;
    values[1] = sum;

    /* Extend to n = 2 … 15 (i.e., 1 … 14 additional rows) */
    for (int step = 1; step <= 14; ++step) {
        size_t tmp_cap = 128, tmp_len = 0;
        state_t *tmp = (state_t *)malloc(tmp_cap * sizeof(state_t));

        for (size_t i = 0; i < cur_len; ++i) {
            uint32_t pair = cur[i].pair;
            int p = pair >> SHIFT;
            int q = pair & ((1 << SHIFT) - 1);
            uint64_t cnt = cur[i].count;

            for (int r = 0; r < ROWS; ++r) {
                if (!good2x3[q * ROWS + r]) continue;
                if (!valid_3x2_triple(p, q, r)) continue;

                uint32_t new_pair = (q << SHIFT) | r;
                if (tmp_len == tmp_cap) {
                    tmp_cap <<= 1;
                    tmp = (state_t *)realloc(tmp, tmp_cap * sizeof(state_t));
                }
                tmp[tmp_len].pair = new_pair;
                tmp[tmp_len].count = cnt;
                ++tmp_len;
            }
        }

        /* Sort and aggregate identical pairs. */
        if (tmp_len > 1)
            qsort(tmp, tmp_len, sizeof(state_t), compare_state);

        size_t write = 0;
        for (size_t i = 0; i < tmp_len; ) {
            uint32_t cur_pair = tmp[i].pair;
            uint64_t acc = 0;
            while (i < tmp_len && tmp[i].pair == cur_pair) {
                acc += tmp[i].count;
                ++i;
            }
            if (write == cur_cap) {
                cur_cap <<= 1;
                cur = (state_t *)realloc(cur, cur_cap * sizeof(state_t));
            }
            cur[write].pair = cur_pair;
            cur[write].count = acc;
            ++write;
        }
        cur_len = write;
        free(tmp);

        /* Sum counts for current n */
        seq_t sum_n = 0;
        for (size_t i = 0; i < cur_len; ++i)
            sum_n += (seq_t)cur[i].count;
        values[step + 1] = sum_n;
    }

    free(cur);
    /* good2x3 not freed; it is small (16 KiB). */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_initial_terms();

    if (n <= 15)
        return values[(size_t)n];

    /* n > 15: use a(n) = 10*a(n‑3) */
    switch (n % 3) {
    case 0: /* start = 15 */
        return seq_mul(values[15], pow10((n - 15) / 3));
    case 1: /* start = 13 */
        return seq_mul(values[13], pow10((n - 13) / 3));
    default: /* n % 3 == 2, start = 14 */
        return seq_mul(values[14], pow10((n - 14) / 3));
    }
}
