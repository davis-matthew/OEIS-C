/*
 * A185795  Number of (n+1)X7 binary arrays with no 2X2 subblock sum equal to
 *          any horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A185795 via dynamic programming on 2‑row blocks. */
/* The sequence counts (n+1)×7 binary arrays where every 2×2 subblock
   sum is different from the sum of its horizontal and vertical neighbours. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum row size (2^7 = 128 possible rows). */
#define ROW_BITS 7
#define ROW_SIZE (1 << ROW_BITS)
#define STATE_COUNT (ROW_SIZE * ROW_SIZE)
#define SUM_LEN 6      /* length of the 2×2 sum vector */

/* Global arrays used by the DP routine. */
static bool horiz_ok[STATE_COUNT];
static uint8_t sumvec[STATE_COUNT][SUM_LEN];
static size_t next_start[STATE_COUNT];
static uint8_t next_len[STATE_COUNT];
static uint8_t *next_all;

/* DP working buffers.  Two buffers are used and swapped after each step. */
static seq_t cur_buf[STATE_COUNT];
static seq_t next_buf[STATE_COUNT];
static seq_t *cur = cur_buf;
static seq_t *next = next_buf;

/* Helper: compute row bits sum for a 2×2 block at position j (0..5). */
static inline uint8_t block_sum(uint8_t a, uint8_t b, int j)
{
    return ((a >> j) & 1) + ((a >> (j + 1)) & 1) +
           ((b >> j) & 1) + ((b >> (j + 1)) & 1);
}

/* Build all data structures used by the DP. */
static void init_data(void)
{
    /* 1. Generate all horizontal‑valid pairs and their sum vectors. */
    for (int a = 0; a < ROW_SIZE; ++a) {
        for (int b = 0; b < ROW_SIZE; ++b) {
            int idx = a * ROW_SIZE + b;
            bool ok = true;
            for (int j = 0; j < SUM_LEN; ++j) {
                uint8_t s = block_sum(a, b, j);
                sumvec[idx][j] = s;
                if (j > 0 && s == sumvec[idx][j - 1]) { /* adjacent equal */
                    ok = false;
                    break;
                }
            }
            horiz_ok[idx] = ok;
        }
    }

    /* 2. First pass: count outgoing transitions for each valid state. */
    size_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (!horiz_ok[s]) continue;
        int a = s / ROW_SIZE;
        int b = s % ROW_SIZE;
        uint8_t cnt = 0;
        for (int c = 0; c < ROW_SIZE; ++c) {
            int s2 = b * ROW_SIZE + c;
            if (!horiz_ok[s2]) continue;
            bool diff = true;
            for (int k = 0; k < SUM_LEN; ++k) {
                if (sumvec[s][k] == sumvec[s2][k]) { diff = false; break; }
            }
            if (diff) ++cnt;
        }
        next_len[s] = cnt;
        total += cnt;
    }

    /* 3. Build CSR structure for adjacency lists. */
    next_all = (uint8_t *)malloc(total * sizeof(uint8_t));
    size_t pos = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (!horiz_ok[s]) continue;
        next_start[s] = pos;
        int a = s / ROW_SIZE;
        int b = s % ROW_SIZE;
        for (int c = 0; c < ROW_SIZE; ++c) {
            int s2 = b * ROW_SIZE + c;
            if (!horiz_ok[s2]) continue;
            bool diff = true;
            for (int k = 0; k < SUM_LEN; ++k) {
                if (sumvec[s][k] == sumvec[s2][k]) { diff = false; break; }
            }
            if (diff) {
                next_all[pos++] = (uint8_t)c;
            }
        }
    }
}

/* Main routine computing the n‑th term of A185795. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static bool initialized = false;
    if (!initialized) {
        init_data();
        initialized = true;
    }

    /* Initialise DP for the first pair of rows. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        cur[s] = horiz_ok[s] ? (seq_t)1 : (seq_t)0;
    }

    /* Perform transitions for rows 3..(n+1). */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero the next buffer. */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            if (!horiz_ok[s]) continue;
            seq_t val = cur[s];
            if (val == 0) continue;
            size_t start = next_start[s];
            uint8_t len = next_len[s];
            for (uint8_t t = 0; t < len; ++t) {
                uint8_t c = next_all[start + t];
                int ns = (s % ROW_SIZE) * ROW_SIZE + c; /* new state (b,c) */
                next[ns] += val;
            }
        }
        /* Swap the buffers. */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum up all final states to get the answer. */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) result += cur[s];

    return result;
}
