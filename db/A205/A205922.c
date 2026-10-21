/*
 * A205922  Number of (n+1)X5 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over row states with local
 *            2x2 constraints
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>   /* for malloc, free */

/* Encode a 5‑tuple of digits 0..3 as an integer 0..1023. */
/* rows_vals[s][j] gives the value in column j of row s. */
static uint8_t rows_vals[1024][5];
/* For each start state s, next_rows_ptr[s] points to an array of
   states t that can follow s.  next_count[s] holds the length of
   that array. */
static uint16_t *next_rows_ptr[1024];
static uint8_t  next_count[1024];

/* Pre‑compute all tables on first use. */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Fill rows_vals. */
    for (int s = 0; s < 1024; ++s) {
        int t = s;
        for (int j = 4; j >= 0; --j) {
            rows_vals[s][j] = t & 3;
            t >>= 2;
        }
    }

    /* First pass: count transitions. */
    for (int s = 0; s < 1024; ++s) {
        uint8_t cnt = 0;
        uint8_t *a = rows_vals[s];
        for (int t = 0; t < 1024; ++t) {
            uint8_t *b = rows_vals[t];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int a0 = a[j];
                int a1 = a[j + 1];
                int b0 = b[j];
                int b1 = b[j + 1];
                int inc = (a1 > a0) + (b1 > a1) + (b0 > b1) + (a0 > b0);
                if (inc != 1) { ok = false; break; }
                if (a0 * b1 == a1 * b0) { ok = false; break; } /* det==0 */
            }
            if (ok) ++cnt;
        }
        next_count[s] = cnt;
        next_rows_ptr[s] = (uint16_t *)malloc(cnt * sizeof(uint16_t));
    }

    /* Second pass: store transitions. */
    for (int s = 0; s < 1024; ++s) {
        int pos = 0;
        uint8_t *a = rows_vals[s];
        for (int t = 0; t < 1024; ++t) {
            uint8_t *b = rows_vals[t];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int a0 = a[j];
                int a1 = a[j + 1];
                int b0 = b[j];
                int b1 = b[j + 1];
                int inc = (a1 > a0) + (b1 > a1) + (b0 > b1) + (a0 > b0);
                if (inc != 1) { ok = false; break; }
                if (a0 * b1 == a1 * b0) { ok = false; break; }
            }
            if (ok) next_rows_ptr[s][pos++] = (uint16_t)t;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_tables();

    /* Two buffers for current / next DP values. */
    static seq_t buf1[1024], buf2[1024];
    seq_t *cur = buf1, *nxt = buf2;

    /* Base: 1 row – each state is reachable once. */
    for (int i = 0; i < 1024; ++i) cur[i] = 1;

    /* Perform n transitions to reach (n+1) rows. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Clear next buffer. */
        for (int i = 0; i < 1024; ++i) nxt[i] = 0;

        for (int s = 0; s < 1024; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            uint8_t cnt = next_count[s];
            uint16_t *nb = next_rows_ptr[s];
            for (int k = 0; k < cnt; ++k) {
                nxt[nb[k]] += val;
            }
        }
        /* Swap buffers. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all ending states. */
    seq_t result = 0;
    for (int i = 0; i < 1024; ++i) result += cur[i];
    return result;
}
