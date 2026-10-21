/*
 * A207807  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding horizontal *
 *            patterns 000 and 001 and vertical patterns 010 and 101
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a linear‑time DP (O(n*R²)), where R = 42
 * is the number of admissible rows (0…1 strings of length 7 without
 * 000 or 001).  All data are statically allocated, so the code is
 * valid C11 and C++17 without dynamic allocation of seq_t objects. */

#include <stdint.h>
#include <stdbool.h>

#define ROWS   42                 /* number of admissible rows */
#define STATES (ROWS * ROWS)      /* ordered pairs of rows           */
#define MAX_NEXT ROWS             /* maximal number of next rows per pair */

/* ------------------------------------- */
/* Precomputed data (initialized on first call) */
static bool init_done = false;
static uint8_t rows[ROWS];   /* valid rows as 7‑bit numbers          */

/* For each pair (i,j) the following holds:
 *   next_list[idx][t]   – index of a row k that may follow rows i,j
 *   next_cnt[idx]       – number of such rows (cnt <= ROWS)
 */
static int next_list[STATES][MAX_NEXT];
static int next_cnt[STATES];

/* DP storage: two buffers that are swapped each iteration. */
/* The buffers are statically allocated; their type is seq_t. */
static seq_t dp_a[STATES];
static seq_t dp_b[STATES];

/* ------------------------------------- */
/* Helper: check that a 7‑bit pattern satisfies the horizontal rule. */
static bool good_row(uint8_t x)
{
    /* forbidden triples are 000 and 001 (values 0 and 1). */
    for (int pos = 0; pos <= 4; ++pos) {
        uint8_t y = (x >> pos) & 7;
        if (y == 0 || y == 1)
            return false;
    }
    return true;
}

/* Helper: check that rows a,b,c do not form a forbidden vertical triple
 * in any column.  rows are given as indices into the rows[] array. */
static inline bool transition_ok(int a, int b, int c)
{
    uint8_t ra = rows[a];
    uint8_t rb = rows[b];
    uint8_t rc = rows[c];
    for (int bit = 0; bit < 7; ++bit) {
        int a_bit = (ra >> bit) & 1;
        int b_bit = (rb >> bit) & 1;
        int c_bit = (rc >> bit) & 1;
        if (a_bit == 0 && c_bit == 0 && b_bit == 1) return false; /* 0 1 0 */
        if (a_bit == 1 && c_bit == 1 && b_bit == 0) return false; /* 1 0 1 */
    }
    return true;
}

/* ------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle n < 3 directly. */
    if (n == 1ULL) {
        return (seq_t)42ULL;                        /* a(1) = 42       */
    }
    if (n == 2ULL) {
        return (seq_t)1764ULL;                      /* 42 * 42 = 1764   */
    }

    /* First call: build rows and transition tables. */
    if (!init_done) {
        /* Enumerate all admissible rows. */
        int r = 0;
        for (uint32_t x = 0; x < (1U << 7); ++x) {
            if (good_row((uint8_t)x)) {
                rows[r++] = (uint8_t)x;
            }
        }
        /* r should be 42. */

        /* Build transition lists for every ordered pair (i,j). */
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                int idx = i * ROWS + j;
                int cnt = 0;
                for (int k = 0; k < ROWS; ++k) {
                    if (transition_ok(i, j, k)) {
                        next_list[idx][cnt++] = k;
                    }
                }
                next_cnt[idx] = cnt;
            }
        }
        init_done = true;
    }

    /* Initialize DP for length = 2: every pair (i,j) occurs once. */
    for (int idx = 0; idx < STATES; ++idx) {
        dp_a[idx] = (seq_t)1ULL;
    }

    /* DP loop: step from length 3 up to n. */
    seq_t total = (seq_t)1764ULL;          /* current total for length = 2 */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero the next buffer. */
        for (int idx = 0; idx < STATES; ++idx) {
            dp_b[idx] = (seq_t)0;
        }

        /* Transition from dp_a to dp_b. */
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                int idx = i * ROWS + j;
                seq_t val = dp_a[idx];
                if (val == (seq_t)0)
                    continue;
                int cnt = next_cnt[idx];
                for (int t = 0; t < cnt; ++t) {
                    int k = next_list[idx][t];
                    int dest = j * ROWS + k;
                    dp_b[dest] += val;
                }
            }
        }

        /* Swap buffers for next iteration. */
        for (int idx = 0; idx < STATES; ++idx) {
            dp_a[idx] = dp_b[idx];
        }

        /* Compute total for this length. */
        seq_t sum = (seq_t)0;
        for (int idx = 0; idx < STATES; ++idx) {
            sum += dp_a[idx];
        }
        total = sum;   /* will finally hold total for n */
    }

    return total;
}
