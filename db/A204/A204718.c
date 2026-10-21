/*
 * A204718  Number of (n+1)X4 0..1 arrays with the permanents of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 binary arrays with 2×2 permanents
 *            nondecreasing rightwards and downwards
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* DP parameters */
#define ROWS 16      /* number of distinct 4‑bit row patterns */
#define CODES 27     /* number of possible triples of permanents (0..2)^3 with nondecreasing order encoded in base‑3 */
#define STATES (ROWS * CODES)

/* tables for transitions between consecutive rows */
static int trans_code[ROWS][ROWS];
static int trans_valid[ROWS][ROWS];
static int decode_p0[CODES];
static int decode_p1[CODES];
static int decode_p2[CODES];
static int tables_initialized = 0;

/* initialise the tables once */
static void init_tables(void)
{
    /* decode tables for 3‑digit base‑3 codes */
    for (int code = 0; code < CODES; ++code) {
        int t = code;
        decode_p0[code] = t % 3;  t /= 3;
        decode_p1[code] = t % 3;  t /= 3;
        decode_p2[code] = t % 3;
    }

    /* transition tables for pairs of rows */
    for (int r = 0; r < ROWS; ++r) {
        for (int s = 0; s < ROWS; ++s) {
            int p[3];
            for (int j = 0; j < 3; ++j) {
                int brj   = (r >> j)   & 1;
                int brj1  = (r >> (j+1)) & 1;
                int bsj   = (s >> j)   & 1;
                int bsj1  = (s >> (j+1)) & 1;
                p[j] = (brj  & bsj1) + (brj1 & bsj);
            }
            int code = p[0] + p[1] * 3 + p[2] * 9;
            trans_code[r][s] = code;
            trans_valid[r][s] = (p[0] <= p[1] && p[1] <= p[2]);
        }
    }
    tables_initialized = 1;
}

seq_t a(idx_t n)
{
    if (!tables_initialized) init_tables();

    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    /* local DP arrays */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    /* zero dp_cur */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)0;

    /* initialise with all valid pairs of rows (first two rows) */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (!trans_valid[r0][r1]) continue;
            int code = trans_code[r0][r1];
            int state = r1 * CODES + code;
            dp_cur[state] += (seq_t)1;
        }
    }

    if (n == 1) {            /* only 2 rows */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < STATES; ++i) sum += dp_cur[i];
        return sum;
    }

    int steps = (int)n - 1;  /* number of additional rows to add */
    for (int step = 1; step <= steps; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == (seq_t)0) continue;
            int old_row = s / CODES;
            int old_code = s % CODES;
            int old_p0 = decode_p0[old_code];
            int old_p1 = decode_p1[old_code];
            int old_p2 = decode_p2[old_code];
            for (int new_row = 0; new_row < ROWS; ++new_row) {
                if (!trans_valid[old_row][new_row]) continue;
                int new_code = trans_code[old_row][new_row];
                int new_p0 = decode_p0[new_code];
                int new_p1 = decode_p1[new_code];
                int new_p2 = decode_p2[new_code];
                if (new_p0 >= old_p0 && new_p1 >= old_p1 && new_p2 >= old_p2) {
                    int new_state = new_row * CODES + new_code;
                    dp_next[new_state] += cnt;
                }
            }
        }
        /* copy next into current for the next iteration */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t sum = (seq_t)0;
    for (int i = 0; i < STATES; ++i) sum += dp_cur[i];
    return sum;
}
