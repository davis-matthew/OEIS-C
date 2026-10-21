/*
 * A207486  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays avoiding the forbidden
 *            horizontal patterns 000 and 011 and the forbidden vertical
 *            patterns 011 and 110. Computed by a transfer‑matrix DP over
 *            pairs of last two rows
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define ROWS   18
#define STATES (ROWS * ROWS)

/* Pre‑computed data (filled on first call) */
static int     row_bits[ROWS];
static int     row_index_by_bits[64];
static bool    vertical_ok[ROWS][ROWS][ROWS];
static int     init_done = 0;

static void init(void) {
    /* Compute all valid 6‑bit rows (horizontal constraints). */
    for (int i = 0; i < 64; ++i) row_index_by_bits[i] = -1;
    int cnt = 0;
    for (int b = 0; b < 64; ++b) {
        bool ok = true;
        for (int col = 0; col <= 3; ++col) {
            int t = (b >> col) & 7;            /* 3 bits window */
            if (t == 0 || t == 3) {           /* 000 or 011 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            row_bits[cnt] = b;
            row_index_by_bits[b] = cnt;
            ++cnt;
        }
    }
    /* The count must be 18; assume it is correct per OEIS data. */

    /* Pre‑compute vertical compatibility for all triples of rows. */
    for (int i = 0; i < ROWS; ++i) {
        int a_bits = row_bits[i];
        for (int j = 0; j < ROWS; ++j) {
            int b_bits = row_bits[j];
            for (int k = 0; k < ROWS; ++k) {
                int c_bits = row_bits[k];
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    int a = (a_bits >> col) & 1;
                    int b = (b_bits >> col) & 1;
                    int c = (c_bits >> col) & 1;
                    if ((a == 0 && b == 1 && c == 1) ||
                        (a == 1 && b == 1 && c == 0)) {
                        ok = false;
                        break;
                    }
                }
                vertical_ok[i][j][k] = ok;
            }
        }
    }
    init_done = 1;
}

/* The required function. */
seq_t a(idx_t n) {
    if (!init_done) init();

    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)18 * (seq_t)18;

    static seq_t dp_curr[STATES];
    static seq_t dp_next[STATES];

    /* Initialise dp_curr for the first two rows. */
    for (int s = 0; s < STATES; ++s) dp_curr[s] = (seq_t)0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp_curr[i * ROWS + j] = (seq_t)1;

    /* Iterate over rows 3..n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_next */
        for (int s = 0; s < STATES; ++s) dp_next[s] = (seq_t)0;

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                seq_t val = dp_curr[i * ROWS + j];
                if (!val) continue;          /* skip zero entries */
                for (int k = 0; k < ROWS; ++k) {
                    if (vertical_ok[i][j][k]) {
                        int ns = j * ROWS + k;
                        dp_next[ns] += val;
                    }
                }
            }
        }

        /* swap buffers */
        for (int s = 0; s < STATES; ++s) dp_curr[s] = dp_next[s];
    }

    /* Sum all valid end states. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s) result += dp_curr[s];
    return result;
}
