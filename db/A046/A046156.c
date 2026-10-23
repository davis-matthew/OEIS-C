/*
 * A046156  Limit set for operation of repeatedly replacing a number with the
 *          sum of the cubes of its digits.
 * https://oeis.org/A046156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   digit_cube_sum_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n-th smallest number which lies on a cycle of the function
 * f(x) = sum of the cubes of the decimal digits of x
 * Offset: 1
 * The sequence consists of all integers that are periodic (fixed points or higher‑order cycles)
 * under repeated application of f.
 */

#include <stdlib.h>   /* malloc, free, calloc */
#include <stdint.h>   /* uint32_t */
#include <stdbool.h>  /* bool */
#include "../utils/digit_cube_sum_idx.h"

static const idx_t MAX_N = 6000;   /* upper bound beyond which no new cycles can appear */

/* build the list of all cycle nodes, sorted ascending */
static void build_limit_set(seq_t **out_arr, size_t *out_len)
{
    /* pre‑compute f(n) for all n up to MAX_N */
    idx_t *f = (idx_t *)malloc((MAX_N + 1) * sizeof(idx_t));
    for (idx_t i = 0u; i <= MAX_N; ++i)
        f[i] = digit_cube_sum_idx(i);

    /* state: 0 = unseen, 1 = on current recursion path, 2 = processed */
    uint8_t *state   = (uint8_t *)calloc(MAX_N + 1, sizeof(uint8_t));
    uint8_t *is_cycle = (uint8_t *)calloc(MAX_N + 1, sizeof(uint8_t));
    idx_t *path = (idx_t *)malloc((MAX_N + 1) * sizeof(idx_t));

    for (idx_t start = 0u; start <= MAX_N; ++start) {
        if (state[start] != 0u) continue;

        idx_t cur = start;
        size_t len = 0u;

        /* follow the chain until meeting a visited node */
        while (state[cur] == 0u) {
            state[cur] = 1u;
            path[len++] = cur;
            cur = f[cur];
        }

        /* if a node on the current path is revisited, we found a cycle */
        if (state[cur] == 1u) {
            size_t pos = 0u;
            while (path[pos] != cur) ++pos;
            for (size_t j = pos; j < len; ++j)
                is_cycle[path[j]] = 1u;
        }

        /* mark the whole traversed path as processed */
        for (size_t j = 0u; j < len; ++j)
            state[path[j]] = 2u;
    }

    /* count cycle nodes */
    size_t cnt = 0u;
    for (idx_t i = 0u; i <= MAX_N; ++i)
        if (is_cycle[i]) ++cnt;

    /* allocate and fill the result array in ascending order */
    seq_t *arr = (seq_t *)malloc(cnt * sizeof(seq_t));
    size_t pos = 0u;
    for (idx_t i = 0u; i <= MAX_N; ++i)
        if (is_cycle[i])
            arr[pos++] = (seq_t)i;   /* cast to seq_t */

    *out_arr = arr;
    *out_len = cnt;

    free(f);
    free(state);
    free(is_cycle);
    free(path);
}

/* external function returning the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* static cache of the full sequence, built on first call */
    static bool   built = false;
    static seq_t *seq   = NULL;
    static size_t seq_len = 0u;

    if (!built) {
        build_limit_set(&seq, &seq_len);
        built = true;
    }

    if (n == 0u || n > seq_len)
        return 0u;   /* out of range – behavior not required by the spec */

    return seq[n - 1u];
}
