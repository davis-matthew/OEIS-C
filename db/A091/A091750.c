/*
 * A091750  Alternating row sums of array A091746 (generalized Stirling2 array
 *          (6,2)).
 * https://oeis.org/A091750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=2..2n} (-1)^k * T(n,k), where T(1,2) = 1 and
 *            T(n,k) = Sum_{p=0..2} binomial(2,p) * ff(4(n-1)+k-p, 2-p) *
 *            T(n-1,k-p), n >= 2, with ff(m,j) the falling factorial (A091746
 *            rows)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t const sz = (size_t)(2 * n + 3);
    seq_t *prev = mem_new(seq_t, sz);
    seq_t *cur = mem_new(seq_t, sz);
    prev[2] = (seq_t)1;
    for (idx_t r = 2; r <= n; ++r) {
        idx_t const pmax = 2 * (r - 1);
        for (idx_t k = 0; k < (idx_t)sz; ++k) cur[k] = (seq_t)0;
        for (idx_t k = 2; k <= 2 * r; ++k) {
            seq_t val = (seq_t)0;
            if (k <= pmax) {
                seq_t t = (seq_t)(4 * (r - 1) + k);
                val += prev[k] * (t * (t - 1));
            }
            if (k - 1 >= 2 && k - 1 <= pmax) {
                seq_t t = (seq_t)(4 * (r - 1) + k - 1);
                val += prev[k - 1] * t * 2;
            }
            if (k - 2 >= 2 && k - 2 <= pmax) {
                val += prev[k - 2];
            }
            cur[k] = val;
        }
        seq_t *tmp = prev; prev = cur; cur = tmp;
    }
    seq_t sum = (seq_t)0;
    for (idx_t k = 2; k <= 2 * n; ++k) {
        if (k % 2 == 0) sum += prev[k];
        else sum -= prev[k];
    }
    mem_free(prev);
    mem_free(cur);
    return sum;
}
