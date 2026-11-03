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
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

/* binary search in a sorted array of seq_t values */
static int binary_search_seq(const seq_t *arr, idx_t len, seq_t key) {
    idx_t lo = 0, hi = len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t v = arr[mid];
        if (v == key) return 1;
        if (v < key) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

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
        seq_t *T = mem_new(seq_t, K);
        if (!T) return;   /* allocation failure */
        for (idx_t i = 0; i < K; ++i) {
            seq_t n = i + 1;                    /* indices start at 1 */
            T[i] = n * (n + 1) * (n + 2) / 6;   /* T(n) = C(n+2,3) */
        }

        /* collect all sums of two distinct tetrahedral numbers */
        seq_t *two_sums = mem_new(seq_t, two_cnt);
        idx_t p = 0;
        for (idx_t i = 0; i + 1 < K; ++i)
            for (idx_t j = i + 1; j < K; ++j)
                two_sums[p++] = T[i] + T[j];
        mem_qsort(two_sums, two_cnt, sizeof(seq_t), cmp_seq);

        /* generate 3‑sums, discarding those also 1‑ or 2‑sums */
        seq_t *triple_sums = mem_new(seq_t, tri_cnt);
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
        mem_free(T);
        mem_free(two_sums);

        if (tcnt == 0) {                            /* unlikely, enlarge */
            mem_free(triple_sums);
            K *= 2;
            continue;
        }

        /* sort and deduplicate 3‑sum list */
        mem_qsort(triple_sums, tcnt, sizeof(seq_t), cmp_seq);
        idx_t uniq_cnt = 0;
        for (idx_t i = 0; i < tcnt; ++i)
            if (i == 0 || triple_sums[i] != triple_sums[i-1])
                triple_sums[uniq_cnt++] = triple_sums[i];

        if (uniq_cnt >= need) {                    /* enough terms */
            seq_t *new_cache = mem_new(seq_t, uniq_cnt);
            mem_copy(new_cache, triple_sums, uniq_cnt * sizeof(seq_t));
            mem_free(triple_sums);
            mem_free(seq_cache);
            seq_cache = new_cache;
            seq_cache_size = uniq_cnt;
            return;
        }

        /* not enough: try with a larger K */
        mem_free(triple_sums);
        K *= 2;
    }
}

/* public function: a(n) returns the nth term (1‑based) */
seq_t a(idx_t n) {
    precompute(n);                 /* ensure cache has at least n terms */
    return seq_cache[n - 1];
}
