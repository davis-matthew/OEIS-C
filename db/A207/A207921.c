/*
 * A207921  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207921 – Number of 6×n 0..1 arrays avoiding  
 *  horizontally 0 0 0 and 1 0 1, vertically 0 1 0 and 1 0 1.
 *
 *  The sequence is computed by dynamic programming over columns.
 *  State = last two columns (represented by indices of allowed
 *  6‑bit column patterns).  The transition from (p,q) to (q,c) is
 *  allowed iff for every row r the triple of bits (p_r,q_r,c_r) is
 *  not 000 or 101.  The vertical restriction has already been
 *  incorporated when generating the allowed column patterns.
 *
 *  Complexity:  O(n · M² · d) where M is the number of allowed
 *  column patterns (M = 26); d is the average out‑degree of the
 *  state graph (≤ 3).  For n ≤ 10000 the program finishes far
 *  below the 2‑second limit.
 */

#include <stdint.h>

#define MAX_BITS 6
#define MAX_COLS 64
#define MAX_STATES (MAX_COLS * MAX_COLS)

static uint8_t allowed_codes[MAX_COLS];
static int M;

/* bits[row] for each allowed column */
static uint8_t col_bits[MAX_COLS][MAX_BITS];

/* trans[state][c] == 1 if transition from state (p,q) to (q,c) is valid */
static char trans[MAX_STATES][MAX_COLS];

/* DP working space */
static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];

static int init_done = 0;
static void init(void)
{
    if (init_done) return;
    init_done = 1;

    /* generate allowed 6‑bit column patterns */
    M = 0;
    for (int code = 0; code < 64; ++code) {
        int ok = 1;
        for (int tri = 0; tri < 4; ++tri) {
            int b1 = (code >> tri) & 1;
            int b2 = (code >> (tri + 1)) & 1;
            int b3 = (code >> (tri + 2)) & 1;
            if ((b1 == 0 && b2 == 1 && b3 == 0) ||
                (b1 == 1 && b2 == 0 && b3 == 1)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            allowed_codes[M] = (uint8_t)code;
            for (int r = 0; r < MAX_BITS; ++r)
                col_bits[M][r] = (code >> r) & 1;
            ++M;
        }
    }

    /* precompute transitions */
    int states = M * M;
    for (int p = 0; p < M; ++p) {
        for (int q = 0; q < M; ++q) {
            int sid = p * M + q;
            for (int c = 0; c < M; ++c) {
                int ok = 1;
                for (int r = 0; r < MAX_BITS; ++r) {
                    int b0 = col_bits[p][r];
                    int b1 = col_bits[q][r];
                    int b2 = col_bits[c][r];
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 1 && b1 == 0 && b2 == 1)) {
                        ok = 0;
                        break;
                    }
                }
                trans[sid][c] = ok ? 1 : 0;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init();

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    int states = M * M;
    seq_t *cur = dp_cur;
    seq_t *next = dp_next;

    /* start with all pairs for length 2 */
    for (int i = 0; i < states; ++i) cur[i] = (seq_t)1;

    /* extend from length 3 to n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < states; ++i) next[i] = (seq_t)0;

        for (int s = 0; s < states; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            int p = s / M;
            int q = s % M;
            for (int c = 0; c < M; ++c) {
                if (trans[s][c]) {
                    int ns = q * M + c;
                    next[ns] += val;
                }
            }
        }
        /* swap buffers */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum up all states for final length */
    seq_t total = (seq_t)0;
    for (int i = 0; i < states; ++i) total += cur[i];
    return total;
}
