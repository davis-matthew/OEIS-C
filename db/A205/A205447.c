/*
 * A205447  (1/n)*A205446(n).
 * https://oeis.org/A205447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A205446(n)/n, where A205446(n) = s(k)-s(j) for the least
 *            k such that n | s(k)-s(j) for some j<k, with
 *            s(t)=Fibonacci(2t-1); computed by search on residues
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 24, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
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
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return (seq_t)0;

    idx_t r2 = 0, r1 = 0;
    idx_t k = 0, j = 0;
    for (idx_t t = 1;; ++t) {
        idx_t r;
        if (t == 1) r = 1 % n;
        else if (t == 2) r = 2 % n;
        else r = (3 * r1 + n - r2) % n;
        if (first[r] != 0) {
            j = first[r];
            k = t;
            break;
        }
        first[r] = t;
        r2 = r1;
        r1 = r;
    }
    free(first);

    seq_t p2 = 0, p1 = 0, sj = 0, sk = 0;
    for (idx_t t = 1; t <= k; ++t) {
        seq_t cur;
        if (t == 1) cur = 1;
        else if (t == 2) cur = 2;
        else cur = 3 * p1 - p2;
        if (t == j) sj = cur;
        if (t == k) sk = cur;
        p2 = p1;
        p1 = cur;
    }
    return (sk - sj) / (seq_t)n;
}
