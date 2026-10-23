/*
 * A159441  Number of n X n arrays of squares of integers summing to 38.
 * https://oeis.org/A159441
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all vectors (c1,…,c6) with * Σ ci·si = 38 (si =
 *            1,4,9,16,25,36), * of the multinomial coefficient (n²)! / (c0!
 *            c1! … c6!), * where c0 = n² – Σ ci. * This coefficient can be
 *            evaluated as a product of binomials: * Π binom( N – Σ_{j<i} cj ,
 *            ci ) with N = n²
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"

static const idx_t squares[6] = {1, 4, 9, 16, 25, 36};

typedef struct {
    uint8_t cnt[6];
} sol_t;

static sol_t *solutions = NULL;
static size_t sol_cnt = 0;
static size_t sol_cap = 0;

static void store_solution(const uint8_t cur[6]) {
    if (sol_cnt == sol_cap) {
        size_t new_cap = (sol_cap == 0) ? 16 : sol_cap * 2;
        sol_t *new_s = (sol_t *)realloc(solutions, new_cap * sizeof(sol_t));
        if (!new_s) return; /* unlikely for small size */
        solutions = new_s;
        sol_cap = new_cap;
    }
    for (int i = 0; i < 6; ++i) {
        solutions[sol_cnt].cnt[i] = cur[i];
    }
    ++sol_cnt;
}

static void dfs_solutions(int idx, int rem, uint8_t cur[6]) {
    if (idx == 6) {
        if (rem == 0) store_solution(cur);
        return;
    }
    int max_cnt = rem / (int)squares[idx];
    for (int cnt = 0; cnt <= max_cnt; ++cnt) {
        cur[idx] = (uint8_t)cnt;
        dfs_solutions(idx + 1, rem - cnt * (int)squares[idx], cur);
    }
}

static void init_solutions(void) {
    if (solutions) return;
    uint8_t cur[6] = {0};
    dfs_solutions(0, 38, cur);
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    init_solutions();

    idx_t N = n * n;
    seq_t total = 0;

    for (size_t s = 0; s < sol_cnt; ++s) {
        seq_t term = 1;
        idx_t remaining = N;

        for (int i = 0; i < 6; ++i) {
            idx_t cnt = (idx_t)solutions[s].cnt[i];
            if (cnt == 0) continue;
            if (cnt > remaining) { term = 0; break; }
            term *= binom(remaining, cnt);
            remaining -= cnt;
        }

        total += term;
    }

    return total;
}
