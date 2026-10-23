/*
 * A125482  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 25×25 matrix with A[i][j] = 1
 *            if |i−j| ≤ 9 (adjacent digits differ by at most 9) and A[i][j] =
 *            0 otherwise. a(0) = 1. The sequence counts circular base‑25
 *            numbers with this property
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#endif
#include <stdbool.h>   /* bool */

#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* --- local static data ------------------------------------------- */
    static bool ready = false;

    /* adjacency matrix A */
    static seq_t A[25 * 25];

    /* two working matrices for successive powers of A */
    static seq_t cur_arr[25 * 25];
    static seq_t next_arr[25 * 25];
    static seq_t *mat_cur = NULL;
    static seq_t *mat_next = NULL;

    /* cache of computed a(k) values */
#if defined(__cplusplus)
    static std::vector<seq_t> seq_cache;
#else
    static seq_t *seq_cache = NULL;
    static size_t seq_cache_len = 0;
    static size_t seq_cache_cap = 0;
#endif

    /* --- initialization ---------------------------------------------- */
    if (!ready) {
        /* build adjacency matrix A */
        for (idx_t i = 0; i < 25; ++i) {
            for (idx_t j = 0; j < 25; ++j) {
                if (i > j)
                    A[i * 25 + j] = ((i - j) <= 9 ? 1 : 0);
                else
                    A[i * 25 + j] = ((j - i) <= 9 ? 1 : 0);
            }
        }

        /* set the working matrices to identity for power 0 */
        mat_cur  = cur_arr;
        mat_next = next_arr;
        for (idx_t i = 0; i < 25; ++i) {
            for (idx_t j = 0; j < 25; ++j) {
                mat_cur[i * 25 + j] = (i == j) ? 1 : 0;
            }
        }

        /* initialise cache with a(0) */
#if defined(__cplusplus)
        seq_cache.reserve(1024);
        seq_cache.push_back((seq_t)1);   /* a(0) = 1 */
#else
        seq_cache_cap = 1;
        seq_cache = (seq_t*)malloc(seq_cache_cap * sizeof(seq_t));
        if (!seq_cache) return 0;         /* allocation failure */
        seq_cache[0] = (seq_t)1;
        seq_cache_len = 1;
#endif
        ready = true;
    }

    /* --- compute terms up to n --------------------------------------- */
    size_t current_len;
#if defined(__cplusplus)
    while ( (current_len = seq_cache.size()) <= n ) {
#else
    while ( (current_len = seq_cache_len) <= n ) {
#endif
        /* compute next = mat_cur * A */
        for (idx_t i = 0; i < 25; ++i) {
            for (idx_t j = 0; j < 25; ++j) {
                seq_t sum = 0;
                for (idx_t k = 0; k < 25; ++k) {
                    if (A[k * 25 + j])  /* adjacency 0/1 */
                        sum += mat_cur[i * 25 + k];
                }
                mat_next[i * 25 + j] = sum;
            }
        }

        /* trace of the new matrix = a(current_len) */
        seq_t trace = 0;
        for (idx_t i = 0; i < 25; ++i)
            trace += mat_next[i * 25 + i];

        /* store the new term */
#if defined(__cplusplus)
        seq_cache.push_back(trace);
#else
        if (seq_cache_len == seq_cache_cap) {
            size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 1;
            seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure */
            seq_cache = tmp;
            seq_cache_cap = new_cap;
        }
        seq_cache[seq_cache_len++] = trace;
#endif

        /* swap working matrices for next iteration */
        seq_t *tmp_mat = mat_cur;
        mat_cur = mat_next;
        mat_next = tmp_mat;
    }

    /* --- return the answer ------------------------------------------- */
#if defined(__cplusplus)
    return seq_cache[n];
#else
    return seq_cache[n];
#endif
}
