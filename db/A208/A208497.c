/*
 * A208497  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes a(n) for OEIS A208497 by dynamic programming over * n ×
 *            4 binary arrays avoiding horizontally 000,001 and vertically
 *            010,111. * The implementation is O(n) with a fixed 16 × 16
 *            states table. * It works with any seq_t/idx_t types provided by
 *            the build
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define COLS 4
#define SIZE (1 << COLS)

/* Pre‑computation tables */
static bool horiz_allowed[SIZE];
static bool next_allowed[SIZE][SIZE][SIZE];
static bool tables_inited = false;

/* Helper: bit of pattern p (0 = leftmost, COLS-1 = rightmost) */
static inline int bit_at(int p, int col) {
    return (p >> (COLS - 1 - col)) & 1;
}

/* Build the tables once */
static void init_tables(void) {
    if (tables_inited) return;
    /* Horizontal constraints for each row */
    for (int p = 0; p < SIZE; ++p) {
        bool ok = true;
        /* triples starting at positions 0 and 1 */
        if ((bit_at(p, 0) == 0 && bit_at(p, 1) == 0) ||
            (bit_at(p, 1) == 0 && bit_at(p, 2) == 0))
            ok = false;
        horiz_allowed[p] = ok;
    }
    /* Triple constraints: for every allowed triplet (r1,r2,r3) */
    for (int r1 = 0; r1 < SIZE; ++r1) {
        if (!horiz_allowed[r1]) {
            for (int r2 = 0; r2 < SIZE; ++r2)
                for (int r3 = 0; r3 < SIZE; ++r3)
                    next_allowed[r1][r2][r3] = false;
            continue;
        }
        for (int r2 = 0; r2 < SIZE; ++r2) {
            if (!horiz_allowed[r2]) {
                for (int r3 = 0; r3 < SIZE; ++r3)
                    next_allowed[r1][r2][r3] = false;
                continue;
            }
            for (int r3 = 0; r3 < SIZE; ++r3) {
                if (!horiz_allowed[r3]) {
                    next_allowed[r1][r2][r3] = false;
                    continue;
                }
                bool ok = true;
                for (int col = 0; col < COLS; ++col) {
                    int b1 = bit_at(r1, col);
                    int b2 = bit_at(r2, col);
                    int b3 = bit_at(r3, col);
                    if ((b1 == 0 && b2 == 1 && b3 == 0) || /* 010 */
                        (b1 == 1 && b2 == 1 && b3 == 1)) { /* 111 */
                        ok = false;
                        break;
                    }
                }
                next_allowed[r1][r2][r3] = ok;
            }
        }
    }
    tables_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    init_tables();

    /* All allowed rows */
    int allowed_rows[SIZE];
    int allowed_cnt = 0;
    for (int p = 0; p < SIZE; ++p)
        if (horiz_allowed[p]) allowed_rows[allowed_cnt++] = p;

    /* Base cases */
    if (n == 1) return (seq_t)allowed_cnt;
    if (n == 2) return (seq_t)allowed_cnt * (seq_t)allowed_cnt;

    /* DP tables */
    static seq_t dp[SIZE][SIZE];
    static seq_t ndp[SIZE][SIZE];

    /* Reset dp to zero */
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            dp[i][j] = 0;

    /* Initial state after first two rows */
    for (int i = 0; i < allowed_cnt; ++i)
        for (int j = 0; j < allowed_cnt; ++j)
            dp[allowed_rows[i]][allowed_rows[j]] = 1;

    /* Transition for rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear ndp */
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                ndp[i][j] = 0;
        for (int r1 = 0; r1 < SIZE; ++r1) {
            if (!horiz_allowed[r1]) continue;
            for (int r2 = 0; r2 < SIZE; ++r2) {
                if (!horiz_allowed[r2]) continue;
                seq_t cur = dp[r1][r2];
                if (cur == 0) continue;
                for (int r3 = 0; r3 < SIZE; ++r3) {
                    if (!horiz_allowed[r3]) continue;
                    if (!next_allowed[r1][r2][r3]) continue;
                    ndp[r2][r3] += cur;
                }
            }
        }
        /* copy ndp to dp */
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all final states */
    seq_t result = 0;
    for (int r1 = 0; r1 < SIZE; ++r1) {
        if (!horiz_allowed[r1]) continue;
        for (int r2 = 0; r2 < SIZE; ++r2) {
            if (!horiz_allowed[r2]) continue;
            result += dp[r1][r2];
        }
    }
    return result;
}
