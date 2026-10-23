/*
 * A196325  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 1,3,0,4,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the 22‑term linear recurrence given in OEIS
 *            (initial values a(1) … a(22) are computed by a
 *            dynamic‑programming search over rows)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* obj definition */ 
/*  (seq_t and idx_t are supplied by the build; no definitions here) */

#define ROWS 625                     /* 5^4 possible rows (4 columns) */
#define MAX_INIT 22                  /* first 22 values are DP‑computed */

static const uint8_t target_of[5] = {1, 3, 0, 4, 2};

/* row representation: digits[row_id][col] */
static uint8_t row_digits[ROWS][4];

/* adjacency list for transitions:
   pair_id = prev * ROWS + cur
   pair_adj[pair_id] -> start, len in adj_next array    */
struct pair_adj_s { size_t start; size_t len; };
static struct pair_adj_s *pair_adj = NULL;   /* length ROWS*ROWS */
static uint16_t *adj_next = NULL;            /* contiguous storage */

/* DP state matrices for current and next row pair counts */
static seq_t dp_cur[ROWS][ROWS];
static seq_t dp_next[ROWS][ROWS];

/* Sequence cache */
static seq_t *seq_cache = NULL;   /* grows as needed */
static size_t seq_cache_len = 0;  /* number of terms stored */

/* Coefficients for n > 22 recurrence */
static const int cof[22] = {
    2, 3, 9, 12, 24, 6, 33, 4, -28, -6, -38, -17,
    3, -17, 2, 2, -2, 2, 2, 2, 1, 2
};

/* -------------------------------------------------------------------------- */
/* Helper: test that a row satisfies its local constraints                   */
/* -------------------------------------------------------------------------- */
static inline bool ok_row(const uint8_t *row,
                          const uint8_t *up,
                          const uint8_t *down,
                          bool hasUp,
                          bool hasDown)
{
    for (int c = 0; c < 4; ++c) {
        uint8_t x = row[c];
        uint8_t t = target_of[x];
        int cnt = 0;
        if (c > 0   && row[c-1] == t) ++cnt;
        if (c < 3   && row[c+1] == t) ++cnt;
        if (hasUp   && up  [c] == t) ++cnt;
        if (hasDown && down[c] == t) ++cnt;
        if ((idx_t)cnt != x) return false;
    }
    return true;
}

/* -------------------------------------------------------------------------- */
/* Initialization: build rows, adjacency and first 22 terms                  */
/* -------------------------------------------------------------------------- */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* 1. Enumerate all row patterns (base‑5). */
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int c = 0; c < 4; ++c) {
            row_digits[r][c] = v % 5;
            v /= 5;
        }
    }

    /* 2. Pre‑compute adjacency lists for transitions. */
    const size_t pair_cnt = ROWS * ROWS;
    pair_adj = (struct pair_adj_s *)malloc(pair_cnt * sizeof(struct pair_adj_s));
    size_t *len_array = (size_t *)calloc(pair_cnt, sizeof(size_t));

    /* First pass: count lengths. */
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            size_t id = prev * ROWS + cur;
            size_t cnt = 0;
            for (int next = 0; next < ROWS; ++next) {
                if (ok_row(row_digits[cur], row_digits[prev],
                           row_digits[next], true, true))
                    ++cnt;
            }
            len_array[id] = cnt;
        }
    }

    /* Compute offsets and total size. */
    size_t total_next = 0;
    for (size_t id = 0; id < pair_cnt; ++id) {
        pair_adj[id].start = total_next;
        pair_adj[id].len   = len_array[id];
        total_next += len_array[id];
    }
    free(len_array);

    adj_next = (uint16_t *)malloc(total_next * sizeof(uint16_t));

    /* Second pass: fill adjacency lists. */
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            size_t id = prev * ROWS + cur;
            size_t pos = pair_adj[id].start;
            for (int next = 0; next < ROWS; ++next) {
                if (ok_row(row_digits[cur], row_digits[prev],
                           row_digits[next], true, true)) {
                    adj_next[pos++] = (uint16_t)next;
                }
            }
        }
    }

    /* 3. Compute first 22 terms by DP over rows. */
    seq_cache = (seq_t *)malloc(MAX_INIT * sizeof(seq_t));
    seq_cache_len = MAX_INIT;

    /* a(1): single row with no neighbors. */
    seq_t a1 = 0;
    for (int r = 0; r < ROWS; ++r) {
        if (ok_row(row_digits[r], NULL, NULL, false, false))
            ++a1;
    }
    seq_cache[0] = a1;

    /* Initialize dp_cur for n = 2 (first pair). */
    memset(dp_cur, 0, sizeof(dp_cur));
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            if (ok_row(row_digits[prev], NULL,
                       row_digits[cur], false, true))
                dp_cur[prev][cur] = 1;
        }
    }

    /* a(2): sum over pairs with bottom row satisfied. */
    seq_t a2 = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            if (dp_cur[prev][cur] &&
                ok_row(row_digits[cur], row_digits[prev],
                       NULL, true, false))
                a2 += dp_cur[prev][cur];
        }
    }
    seq_cache[1] = a2;

    /* DP for n = 3 .. 22. */
    for (size_t n = 3; n <= MAX_INIT; ++n) {
        /* Zero dp_next. */
        memset(dp_next, 0, sizeof(dp_next));

        /* Transition step. */
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t val = dp_cur[prev][cur];
                if (!val) continue;
                size_t id = prev * ROWS + cur;
                size_t pos = pair_adj[id].start;
                size_t len = pair_adj[id].len;
                for (size_t k = 0; k < len; ++k) {
                    int next = adj_next[pos + k];
                    dp_next[cur][next] += val;
                }
            }
        }

        /* Compute a(n). */
        seq_t an = 0;
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                if (dp_next[prev][cur] &&
                    ok_row(row_digits[cur], row_digits[prev],
                           NULL, true, false))
                    an += dp_next[prev][cur];
            }
        }
        seq_cache[n-1] = an;

        /* Prepare for next iteration. */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }
}

/* -------------------------------------------------------------------------- */
/* Main routine: returns a(n) for any n >= 1.                                 */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 */

    init_sequence();               /* ensure everything is ready */

    size_t idx = (size_t)n;
    /* Extend cache if necessary using recurrence. */
    while (seq_cache_len < idx) {
        /* Compute a(next) from previous 22 terms. */
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 22; ++k) {
            int c = cof[k];
            seq_t val = seq_cache[seq_cache_len - k - 1];
            if (c > 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }
        seq_t next_term = pos - neg;   /* sequence is non‑negative */

        seq_cache_len++;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cache_len * sizeof(seq_t));
        seq_cache[seq_cache_len - 1] = next_term;
    }

    return seq_cache[idx - 1];
}
