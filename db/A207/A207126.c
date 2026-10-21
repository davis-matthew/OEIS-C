/*
 * A207126  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207126: number of 6×n 0/1 arrays avoiding
   horizontal patterns 0 0 1 and 0 1 0, and vertical patterns 0 0 1 and 0 1 1.
   The code builds the automaton of valid columns and counts column
   sequences by dynamic programming.                                                    */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static void init_207126(void);
static const uint8_t *get_columns(void);
static const size_t *get_col_counts(void);
static size_t get_num_cols(void);
static const size_t *get_adjacency(const size_t *lens);

static bool init_done = false;

enum { MAX_COLS = 64 }; /* 6-bit columns */

/* Storage for columns and adjacency lists */
static uint8_t col_vals[MAX_COLS];          /* valid column bit patterns */
static size_t col_index[MAX_COLS];          /* mapping from 6-bit value to index */
static size_t num_cols;                     /* number of valid columns */

static size_t adj_len[MAX_COLS][MAX_COLS];  /* number of admissible third columns */
static size_t adj_next[MAX_COLS][MAX_COLS][MAX_COLS];
/* adj_next[i][j] -> list of k indices such that (i,j,k) horizontal safe */

/*--- initialization -----------------------------------------------------*/
static void init_207126(void)
{
    if (init_done) return;
    init_done = true;

    /* find all columns that avoid vertical patterns 001 and 011 */
    num_cols = 0;
    for (uint8_t v = 0; v < 64; ++v) {
        bool good = true;
        for (int i = 0; i <= 3; ++i) {
            uint8_t bits = (v >> i) & 7;           /* triple of rows i,i+1,i+2 */
            if (bits == 1 || bits == 3) {          /* 001 or 011 */
                good = false;
                break;
            }
        }
        if (good) {
            col_index[v] = num_cols;
            col_vals[num_cols] = v;
            num_cols++;
        }
    }

    /* pre‑compute horizontal compatibility among triples of columns */
    for (size_t i = 0; i < num_cols; ++i) {
        for (size_t j = 0; j < num_cols; ++j) {
            adj_len[i][j] = 0;
            for (size_t k = 0; k < num_cols; ++k) {
                bool ok = true;
                /* check all 6 rows */
                for (int r = 0; r < 6; ++r) {
                    uint8_t a = (col_vals[i] >> r) & 1;
                    uint8_t b = (col_vals[j] >> r) & 1;
                    uint8_t c = (col_vals[k] >> r) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 0 && b == 1 && c == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    adj_next[i][j][adj_len[i][j]++] = k;
                }
            }
        }
    }
}

/*--- public function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    init_207126();                          /* set up static data */

    if (n == 1) {
        return (seq_t)num_cols;
    }
    if (n == 2) {
        return (seq_t)num_cols * (seq_t)num_cols;
    }

    size_t states = num_cols * num_cols;
    seq_t *dp_cur = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(states, sizeof(seq_t));
    if (!dp_cur || !dp_next) { /* allocation failure: return 0 */
        free(dp_cur);
        free(dp_next);
        return (seq_t)0;
    }

    /* initial sequences of length 2: one pair of columns */
    for (size_t i = 0; i < num_cols; ++i) {
        for (size_t j = 0; j < num_cols; ++j) {
            dp_cur[i * num_cols + j] = (seq_t)1;
        }
    }

    /* DP for columns 3 .. n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero next array */
        for (size_t s = 0; s < states; ++s) dp_next[s] = (seq_t)0;

        for (size_t i = 0; i < num_cols; ++i) {
            for (size_t j = 0; j < num_cols; ++j) {
                seq_t cnt = dp_cur[i * num_cols + j];
                if (!cnt) continue;
                size_t L = adj_len[i][j];
                for (size_t t = 0; t < L; ++t) {
                    size_t k = adj_next[i][j][t];
                    dp_next[j * num_cols + k] += cnt;
                }
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all counts */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < states; ++s) result += dp_cur[s];

    free(dp_cur);
    free(dp_next);
    return result;
}
