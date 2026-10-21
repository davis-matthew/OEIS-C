/*
 * A233946  Number of (n+1)X(5+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 4.
 * https://oeis.org/A233946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays of 0..2 with every 2×2 subblock
 *            * having the sum of the absolute values of the edge differences
 *            equal to 4
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_seq_zero.h"

/* Number of possible rows: 3^6 = 729 */
#define ROWS 729

/* Precomputed digits for each row value (base‑3 expansion). */
static int row_digits[ROWS][6];

/* Adjacency lists: neigh[i][k] is the k‑th compatible row with row i. */
static uint16_t neigh[ROWS][ROWS];
static uint16_t neigh_cnt[ROWS];

/* Build the pre‑computed tables. */
static void init_tables(void)
{
    /* Decode all row values once. */
    for (uint16_t v = 0; v < ROWS; ++v) {
        uint16_t t = v;
        for (int k = 0; k < 6; ++k) {
            row_digits[v][k] = t % 3;
            t /= 3;
        }
    }

    /* Build adjacency lists. */
    for (uint16_t a = 0; a < ROWS; ++a) {
        uint16_t cnt = 0;
        const int *A = row_digits[a];
        for (uint16_t b = 0; b < ROWS; ++b) {
            const int *B = row_digits[b];
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                int diff = 0;
                diff += (A[j] > A[j+1]) ? A[j] - A[j+1] : A[j+1] - A[j];
                diff += (B[j] > B[j+1]) ? B[j] - B[j+1] : B[j+1] - B[j];
                diff += (A[j] > B[j])   ? A[j] - B[j]   : B[j] - A[j];
                diff += (A[j+1] > B[j+1])? A[j+1] - B[j+1] : B[j+1] - A[j+1];
                if (diff != 4) ok = false;
            }
            if (ok) neigh[a][cnt++] = b;
        }
        neigh_cnt[a] = cnt;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Lazily initialize tables and buffers. */
    static bool init = false;
    static seq_t *cur = NULL;
    static seq_t *next = NULL;
    if (!init) {
        init_tables();
        cur  = alloc_seq_zero(ROWS);
        next = alloc_seq_zero(ROWS);
        init = true;
    }

    /* Base: arrays of one row. Every row is allowed. */
    for (int i = 0; i < ROWS; ++i) cur[i] = (seq_t)1;

    /* For n==0 (though offset starts at 1), return 1. */
    if (n == 0) return (seq_t)1;

    /* Iterate n times to build arrays of length n+1 rows. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero next buffer. */
        for (int i = 0; i < ROWS; ++i) next[i] = (seq_t)0;

        /* Transition step. */
        for (int i = 0; i < ROWS; ++i) {
            if (cur[i] == (seq_t)0) continue;
            seq_t val = cur[i];
            uint16_t cnt = neigh_cnt[i];
            uint16_t *list = neigh[i];
            for (uint16_t k = 0; k < cnt; ++k) {
                next[list[k]] += val;
            }
        }

        /* If this is the final step, sum and return. */
        if (step == n) {
            seq_t total = (seq_t)0;
            for (int i = 0; i < ROWS; ++i) total += next[i];
            return total;
        }

        /* Prepare for next iteration. */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
