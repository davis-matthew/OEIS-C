/*
 * A059160  Number of ordered ways of writing n as a sum of 5 generalized
 *          pentagonal numbers (A001318).
 * https://oeis.org/A059160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 5‑tuples of generalized pentagonal
 *            numbers summing to n
 * Range:     u8 n <= 13, u16 n <= 558, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059160 – ordered ways of writing n as a sum of 5 generalized pentagonal numbers */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- generate generalized pentagonal numbers ≤ n ---------- */
    size_t pn_cnt = 0;
    /* maximum possible number of pentagonal numbers up to n is small (≈√n) */
    /* allocate temporary storage for them */
    idx_t *pent = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!pent) return 0;  /* Out of memory – returns 0 in this unlikely case */

    /* p(k) = k(3k-1)/2  for k = 0,1,2,…   (non‑negative part) */
    for (idx_t k = 0;; ++k) {
        idx_t p = k * (3 * k - 1) / 2;
        if (p > n) break;
        pent[pn_cnt++] = p;
    }
    /* p(k) = k(3k+1)/2  for k = 1,2,3,…   (negative part) */
    for (idx_t k = 1;; ++k) {
        idx_t p = k * (3 * k + 1) / 2;
        if (p > n) break;
        pent[pn_cnt++] = p;
    }

    /* ---------- dynamic programming for 5‑fold convolution ---------- */
    size_t len = (size_t)n + 1;
    seq_t *prev = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *cur  = (seq_t*)calloc(len, sizeof(seq_t));
    if (!prev || !cur) {
        free(pent);
        free(prev); free(cur);
        return 0;   /* Out of memory */
    }

    prev[0] = 1;  /* 0 can be written as 0 summed 0 times */

    for (int t = 1; t <= 5; ++t) {
        /* clear current array */
        for (size_t i = 0; i < len; ++i) cur[i] = 0;

        /* perform one convolution step with the pentagonal numbers */
        for (size_t pi = 0; pi < pn_cnt; ++pi) {
            size_t p = (size_t)pent[pi];
            for (size_t s = p; s < len; ++s) {
                cur[s] += prev[s - p];
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[(size_t)n];

    /* ---------- clean up ---------- */
    free(pent);
    free(prev);
    free(cur);

    return result;
}
