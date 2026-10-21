/*
 * A207679  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 27, u64 n <= 64, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×5 binary arrays avoiding
   0 0 0 and 1 0 1 horizontally and
   0 0 1 and 1 1 1 vertically.
   Solution is a DP over pairs of rows. */

#include <stdbool.h>
#include <stdint.h>

#define ROWS   32
#define STATES (ROWS * ROWS)

/* Pre‑computed tables */
static bool init_done       = false;
static bool allowed_row[ROWS];
static uint8_t adj_count[STATES];
static uint8_t adj_list[STATES][ROWS];

/* Is a single row compatible with horizontal constraints? */
static inline bool row_ok(int r)
{
    for (int k = 0; k <= 2; ++k) {
        int b2 = (r >> (k + 2)) & 1;
        int b1 = (r >> (k + 1)) & 1;
        int b0 = (r >> k) & 1;
        if ((b2 == 0 && b1 == 0 && b0 == 0) ||
            (b2 == 1 && b1 == 0 && b0 == 1))
            return false;
    }
    return true;
}

/* Is a triple of consecutive rows compatible with vertical constraints? */
static inline bool vertical_ok(int a, int b, int c)
{
    for (int j = 0; j < 5; ++j) {
        int va = (a >> j) & 1;
        int vb = (b >> j) & 1;
        int vc = (c >> j) & 1;
        if ((va == 0 && vb == 0 && vc == 1) ||
            (va == 1 && vb == 1 && vc == 1))
            return false;
    }
    return true;
}

/* Build transition tables on first use */
static void build_tables(void)
{
    for (int r = 0; r < ROWS; ++r)
        allowed_row[r] = row_ok(r);

    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int s = (a << 5) | b;            /* state (a,b) */
            uint8_t count = 0;
            if (allowed_row[a] && allowed_row[b]) {
                for (int c = 0; c < ROWS; ++c)
                    if (allowed_row[c] && vertical_ok(a, b, c))
                        adj_list[s][count++] = (uint8_t)c;
            }
            adj_count[s] = count;
        }
    }
    init_done = true;
}

/* DP tables */
static seq_t dp_prev[STATES];
static seq_t dp_cur[STATES];

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* outside defined offset */

    if (!init_done)
        build_tables();

    if (n == 1)
        return (seq_t)13;

    /* Initialise dp_prev: all allowed pairs are counted as 1. */
    const seq_t zero = 0;
    for (int s = 0; s < STATES; ++s)
        dp_prev[s] = zero;
    for (int r1 = 0; r1 < ROWS; ++r1) {
        if (!allowed_row[r1]) continue;
        for (int r2 = 0; r2 < ROWS; ++r2) {
            if (!allowed_row[r2]) continue;
            dp_prev[(r1 << 5) | r2] = 1;
        }
    }

    if (n == 2) {
        seq_t sum = zero;
        for (int s = 0; s < STATES; ++s)
            sum += dp_prev[s];
        return sum;                      /* 13*13 = 169 */
    }

    /* DP for rows 3 .. n */
    for (idx_t i = 3; i <= n; ++i) {
        for (int s = 0; s < STATES; ++s)
            dp_cur[s] = zero;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_prev[s];
            if (val == zero)
                continue;
            int b = s & 31;              /* last row of the state */
            uint8_t *list = adj_list[s];
            uint8_t count = adj_count[s];
            for (uint8_t k = 0; k < count; ++k) {
                int c = list[k];
                int idx = (b << 5) | c;   /* new state (b,c) */
                dp_cur[idx] += val;
            }
        }

        /* swap dp_prev and dp_cur */
        for (int s = 0; s < STATES; ++s)
            dp_prev[s] = dp_cur[s];
    }

    /* Sum all states after processing n rows */
    seq_t total = zero;
    for (int s = 0; s < STATES; ++s)
        total += dp_prev[s];
    return total;
}
