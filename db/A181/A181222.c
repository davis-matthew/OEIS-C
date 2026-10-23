/*
 * A181222  Number of nX6 binary matrices with no three 1's adjacent in a line
 *          horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A181222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary matrices with no three adjacent 1's
 *            in any horizontal, vertical, diagonal or anti‑diagonal line. The
 *            code computes a(n) by dynamic programming over the last two
 *            rows. State size = number of 6‑bit rows with no horizontal “111”
 *            = 44. Transition from pair (r1,r2) to r3 is...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   has111
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence matches OEIS A181222. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has111.h"

static inline bool triple_ok(int a, int b, int c) {
    if ((a & b & c) != 0) return false; /* vertical triple */
    for (int shift = 0; shift <= 3; ++shift) {
        if (((a >> shift) & 1) &&
            ((b >> (shift + 1)) & 1) &&
            ((c >> (shift + 2)) & 1))
            return false;                 /* main diagonal triple */
        if (((a >> (shift + 2)) & 1) &&
            ((b >> (shift + 1)) & 1) &&
            ((c >> shift) & 1))
            return false;                 /* anti‑diagonal triple */
    }
    return true;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static int NUM;
    static int *allowedRows;
    static int idxOf[64];
    static int *nextCount;
    static int **nextRows;
    static seq_t *dp;
    static seq_t *nextDP;

    if (!initialized) {
        /* Enumerate all 6‑bit rows with no horizontal “111”. */
        int count = 0;
        for (int p = 0; p < 64; ++p) idxOf[p] = -1;
        for (int p = 0; p < 64; ++p) {
            if (!has111(p)) {
                if (count == 0) allowedRows = (int*)calloc(64, sizeof(int));
                allowedRows[count] = p;
                idxOf[p] = count;
                ++count;
            }
        }
        NUM = count;  /* should be 44 */
        /* Transition tables for (r1,r2) → list of possible r3. */
        int totalPairs = NUM * NUM;
        nextCount = (int*)calloc(totalPairs, sizeof(int));
        nextRows = (int**)malloc(totalPairs * sizeof(int*));
        for (int i = 0; i < NUM; ++i) {
            int a = allowedRows[i];
            for (int j = 0; j < NUM; ++j) {
                int b = allowedRows[j];
                int idx = i * NUM + j;
                int cnt = 0;
                for (int k = 0; k < NUM; ++k) {
                    int c = allowedRows[k];
                    if (triple_ok(a, b, c)) ++cnt;
                }
                nextCount[idx] = cnt;
                if (cnt > 0) {
                    nextRows[idx] = (int*)malloc(cnt * sizeof(int));
                    int pos = 0;
                    for (int k = 0; k < NUM; ++k) {
                        int c = allowedRows[k];
                        if (triple_ok(a, b, c)) nextRows[idx][pos++] = k;
                    }
                } else {
                    nextRows[idx] = NULL;
                }
            }
        }
        /* Allocate DP arrays. */
#if defined(__cplusplus)
        dp     = new seq_t[NUM * NUM]();
        nextDP = new seq_t[NUM * NUM]();
#else
        dp     = (seq_t*)calloc(NUM * NUM, sizeof(seq_t));
        nextDP = (seq_t*)calloc(NUM * NUM, sizeof(seq_t));
#endif
        initialized = true;
    }

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)NUM;                 /* 44 */
    if (n == 2) return (seq_t)NUM * (seq_t)NUM;     /* 1936 */

    size_t len = NUM * NUM;
    /* Initialize DP for 2 rows: all pairs are allowed independently. */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    for (int i = 0; i < NUM; ++i)
        for (int j = 0; j < NUM; ++j)
            dp[i * NUM + j] = 1;

    /* DP for rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero nextDP */
        for (size_t i = 0; i < len; ++i) nextDP[i] = 0;

        for (int i = 0; i < NUM; ++i) {
            for (int j = 0; j < NUM; ++j) {
                seq_t val = dp[i * NUM + j];
                if (val == 0) continue;
                int idx = i * NUM + j;
                int cnt = nextCount[idx];
                int *lst = nextRows[idx];
                for (int t = 0; t < cnt; ++t) {
                    int k = lst[t];
                    nextDP[j * NUM + k] += val;
                }
            }
        }
        /* swap dp and nextDP */
        seq_t *tmp = dp; dp = nextDP; nextDP = tmp;
    }

    /* Sum all states to get the final count. */
    seq_t total = 0;
    for (size_t i = 0; i < len; ++i) total += dp[i];
    return total;
}
