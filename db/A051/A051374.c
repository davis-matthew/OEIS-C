/*
 * A051374  Numbers not of form n + sum of digits of n^2.
 * https://oeis.org/A051374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n that cannot be written as k + sum_of_digits(k^2) for
 *            any k
 * Range:     u8 n <= 10, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *uncovered   = NULL;
    static size_t uncovered_len = 0, uncovered_cap = 0;

    static uint8_t *covered   = NULL;
    static size_t covered_len = 0;

    static idx_t processed_n   = 0;
    static idx_t next_candidate = 1;

    /* Return already computed value if possible */
    if (uncovered_len >= n) return uncovered[n-1];

    /* Grow the sequence until we have at least n terms */
    while (uncovered_len < n) {
        /* process next k */
        ++processed_n;
        seq_t k = (seq_t)processed_n;
        seq_t sq  = k * k;
        seq_t sum = digit_sum(sq);
        seq_t v   = k + sum;

        /* ensure the covered array can index v */
        size_t v_idx = (size_t)v;
        if (v_idx >= covered_len) {
            size_t new_len = v_idx + 1;
            uint8_t *tmp = (uint8_t *)realloc(covered, new_len);
            if (!tmp) return (seq_t)0;        /* allocation failure (unlikely) */
            for (size_t i = covered_len; i < new_len; ++i) tmp[i] = 0;
            covered = tmp;
            covered_len = new_len;
        }
        covered[v_idx] = 1;

        /* Scan uncovered candidates up to the current processed_n */
        while (next_candidate <= processed_n && uncovered_len < n) {
            size_t cand = (size_t)next_candidate;
            bool is_cov = (cand < covered_len && covered[cand] != 0);
            if (!is_cov) {
                /* grow uncovered array if necessary */
                if (uncovered_len >= uncovered_cap) {
                    size_t new_cap = uncovered_cap ? uncovered_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(uncovered, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;   /* allocation failure */
                    uncovered = tmp;
                    uncovered_cap = new_cap;
                }
                uncovered[uncovered_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return uncovered[n-1];
}
