/*
 * A102802  Let f(n) be the minimal number of distinct nonzero tetrahedral
 *          numbers that add to n (or -1 if n is not a sum of distinct
 *          tetrahedral numbers); sequence gives numbers n for which f(n) = 3.
 * https://oeis.org/A102802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) gives the nth integer that can be expressed as the sum of
 *            exactly three distinct non‑zero tetrahedral numbers, but cannot
 *            be expressed as one or two such numbers. The function builds the
 *            list by generating tetrahedral numbers, collecting all 3‑sums,
 *            filtering those that are also 1‑ or 2‑sums...
 * Range:     u8 none, u16 none, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binary_search_seq, cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/binary_search_seq.h"
#include "../utils/cmp_seq.h"

/* compare two seq_t values for qsort */

/* static cache of the generated sequence */
static seq_t *seq_cache    = NULL;
static idx_t seq_cache_size = 0;

/* precompute the sequence up to at least 'need' terms */
static void precompute(idx_t need) {
    if (seq_cache_size >= need) return;            /* already enough */

    idx_t K = 200;                                 /* initial # tetrahedrals */
    while (1) {
        idx_t two_cnt  = K * (K - 1) / 2;
        idx_t tri_cnt  = K * (K - 1) * (K - 2) / 6;
        if (tri_cnt == 0) { K *= 2; continue; }   /* too small */

        /* allocate tetrahedral numbers */
        seq_t *T = (seq_t *)malloc(K * sizeof(seq_t));
        if (!T) return;   /* allocation failure */
        for (idx_t i = 0; i < K; ++i) {
            seq_t n = i + 1;                    /* indices start at 1 */
            T[i] = n * (n + 1) * (n + 2) / 6;   /* T(n) = C(n+2,3) */
        }

        /* collect all sums of two distinct tetrahedral numbers */
        seq_t *two_sums = (seq_t *)malloc(two_cnt * sizeof(seq_t));
        idx_t p = 0;
        for (idx_t i = 0; i + 1 < K; ++i)
            for (idx_t j = i + 1; j < K; ++j)
                two_sums[p++] = T[i] + T[j];
        qsort(two_sums, two_cnt, sizeof(seq_t), cmp_seq);

        /* generate 3‑sums, discarding those also 1‑ or 2‑sums */
        seq_t *triple_sums = (seq_t *)malloc(tri_cnt * sizeof(seq_t));
        idx_t tcnt = 0;
        for (idx_t i = 0; i + 2 < K; ++i) {
            for (idx_t j = i + 1; j + 1 < K; ++j) {
                for (idx_t k = j + 1; k < K; ++k) {
                    seq_t sum = T[i] + T[j] + T[k];
                    if (binary_search_seq(T, K, sum)) continue;           /* 1‑sum */
                    if (binary_search_seq(two_sums, two_cnt, sum)) continue; /* 2‑sum */
                    triple_sums[tcnt++] = sum;
                }
            }
        }
        free(T);
        free(two_sums);

        if (tcnt == 0) {                            /* unlikely, enlarge */
            free(triple_sums);
            K *= 2;
            continue;
        }

        /* sort and deduplicate 3‑sum list */
        qsort(triple_sums, tcnt, sizeof(seq_t), cmp_seq);
        idx_t uniq_cnt = 0;
        for (idx_t i = 0; i < tcnt; ++i)
            if (i == 0 || triple_sums[i] != triple_sums[i-1])
                triple_sums[uniq_cnt++] = triple_sums[i];

        if (uniq_cnt >= need) {                    /* enough terms */
            seq_t *new_cache = (seq_t *)malloc(uniq_cnt * sizeof(seq_t));
            memcpy(new_cache, triple_sums, uniq_cnt * sizeof(seq_t));
            free(triple_sums);
            free(seq_cache);
            seq_cache = new_cache;
            seq_cache_size = uniq_cnt;
            return;
        }

        /* not enough: try with a larger K */
        free(triple_sums);
        K *= 2;
    }
}

/* public function: a(n) returns the nth term (1‑based) */
seq_t a(idx_t n) {
    precompute(n);                 /* ensure cache has at least n terms */
    return seq_cache[n - 1];
}
