/*
 * A106674  Marks on lexicographically second 7-mark optimal Golomb ruler.
 * https://oeis.org/A106674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the marks of the lexicographically second 7‑mark optimal
   Golomb ruler by enumerating all optimal rulers of minimal length. */

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#define RULER_SIZE 7

static seq_t second_ruler[RULER_SIZE];

/*--- Backtracking to enumerate all optimal 7‑mark rulers of minimal length ---*/
static void backtrack(int idx,
                      int marks[],
                      bool used[],
                      int *pbest_len,
                      int *sol_cnt,
                      int sol[][RULER_SIZE])
{
    if (idx == RULER_SIZE) {
        int len = marks[RULER_SIZE - 1];
        if (len < *pbest_len) {
            *pbest_len = len;
            *sol_cnt = 1;
            memcpy(sol[0], marks, RULER_SIZE * sizeof(int));
        } else if (len == *pbest_len) {
            if (*sol_cnt < 10)  /* safe upper bound for the number of solutions */
                memcpy(sol[*sol_cnt], marks, RULER_SIZE * sizeof(int));
            (*sol_cnt)++;
        }
        return;
    }

    int start = marks[idx - 1] + 1;
    int max_start = (*pbest_len == INT_MAX) ? 30 : *pbest_len; /* safe upper bound */
    for (int v = start; v <= max_start; ++v) {
        if (*pbest_len != INT_MAX && v + (RULER_SIZE - idx - 1) > *pbest_len)
            break;                                   /* prune impossible finals */

        bool conflict = false;
        for (int i = 0; i < idx; ++i) {
            int d = v - marks[i];
            if (used[d]) { conflict = true; break; }
        }
        if (conflict) continue;

        marks[idx] = v;
        for (int i = 0; i < idx; ++i) used[v - marks[i]] = true;

        backtrack(idx + 1, marks, used, pbest_len, sol_cnt, sol);

        for (int i = 0; i < idx; ++i) used[v - marks[i]] = false;
    }
}

/*--- Compute the lexicographically second optimal ruler once ---*/
static void compute_second_ruler(void)
{
    int marks[RULER_SIZE];
    bool used[50] = {false};            /* differences up to about 25 */
    int best_len = INT_MAX;
    int sol_cnt = 0;
    int solutions[10][RULER_SIZE] = {0};

    marks[0] = 0;
    backtrack(1, marks, used, &best_len, &sol_cnt, solutions);

    /* Sort solutions lexicographically */
    for (int i = 0; i < sol_cnt - 1; ++i) {
        for (int j = i + 1; j < sol_cnt; ++j) {
            bool need_swap = false;
            for (int k = 0; k < RULER_SIZE; ++k) {
                if (solutions[i][k] < solutions[j][k]) { need_swap = false; break; }
                if (solutions[i][k] > solutions[j][k]) { need_swap = true; break; }
            }
            if (need_swap) {
                int tmp[RULER_SIZE];
                memcpy(tmp, solutions[i], RULER_SIZE * sizeof(int));
                memcpy(solutions[i], solutions[j], RULER_SIZE * sizeof(int));
                memcpy(solutions[j], tmp, RULER_SIZE * sizeof(int));
            }
        }
    }

    if (sol_cnt >= 2) {
        for (int i = 0; i < RULER_SIZE; ++i)
            second_ruler[i] = (seq_t)solutions[1][i];
    } else {
        for (int i = 0; i < RULER_SIZE; ++i)
            second_ruler[i] = 0;    /* fallback */
    }
}

/*--- Public sequence function ---*/
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_second_ruler();
        initialized = true;
    }
    return (n < (idx_t)RULER_SIZE) ? second_ruler[(size_t)n] : 0;
}
