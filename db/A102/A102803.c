/*
 * A102803  Let f(n) be the minimal number of distinct nonzero tetrahedral
 *          numbers that add to n (or -1 if n is not a sum of distinct
 *          tetrahedral numbers); sequence gives numbers n for which f(n) = 4.
 * https://oeis.org/A102803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102803:
   a(n) = the n-th integer for which the minimal number of distinct non‑zero
   tetrahedral numbers summing to that integer is exactly 4.
   (Computed by a direct DP over tetrahedral numbers.) */

#include <stdint.h>
#include <stdlib.h>

static seq_t *reachable = NULL;
static idx_t reachable_size = 0;
static seq_t *answers = NULL;
static idx_t ans_count = 0;
static idx_t ans_capacity = 0;
static idx_t pre_limit = 0;

/* Ensure we have at least needed answers precomputed. */
static void precompute(idx_t needed) {
    if (ans_count >= needed) return;

    /* Start with a reasonable limit. */
    idx_t limit = pre_limit ? pre_limit : 200000;

    while (1) {
        /* Allocate/resize reachable array. */
        if (reachable_size < limit + 1) {
            seq_t *new_r = (seq_t *)realloc(reachable, (limit + 1) * sizeof(seq_t));
            reachable = new_r;
            reachable_size = limit + 1;
        }

        /* Compute tetrahedral numbers up to limit. */
        idx_t *tetra = NULL;
        idx_t tetra_cap = 0;
        idx_t tetra_cnt = 0;
        for (idx_t k = 1;; ++k) {
            seq_t t = (seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2) / 6;
            if (t > limit) break;
            if (tetra_cnt == tetra_cap) {
                idx_t new_cap = tetra_cap ? tetra_cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(tetra, new_cap * sizeof(idx_t));
                tetra = tmp;
                tetra_cap = new_cap;
            }
            tetra[tetra_cnt++] = (idx_t)t;
        }

        /* Initialize reachable array. */
        const seq_t INF = 9999;
        for (idx_t i = 0; i <= limit; ++i) reachable[i] = INF;
        reachable[0] = 0;

        /* DP over distinct tetrahedral numbers. */
        for (idx_t i = 0; i < tetra_cnt; ++i) {
            idx_t t = tetra[i];
            for (idx_t n = limit; n >= t; --n) {
                seq_t prev = reachable[n - t];
                if (prev + 1 < reachable[n]) reachable[n] = prev + 1;
            }
        }

        /* Collect answers. */
        if (ans_capacity == 0) {
            ans_capacity = 1024;
            answers = (seq_t *)malloc(ans_capacity * sizeof(seq_t));
        }
        idx_t new_count = 0;
        for (idx_t n = 1; n <= limit; ++n) {
            if (reachable[n] == 4) {
                if (new_count == ans_capacity) {
                    ans_capacity *= 2;
                    seq_t *tmp = (seq_t *)realloc(answers, ans_capacity * sizeof(seq_t));
                    answers = tmp;
                }
                answers[new_count++] = (seq_t)n;
            }
        }

        ans_count = new_count;
        pre_limit = limit;

        /* Free temporary array. */
        free(tetra);

        if (ans_count >= needed) break;
        /* Not enough: increase limit and recompute. */
        limit = limit * 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but return 0 */
    precompute(n);
    return answers[n - 1];
}
