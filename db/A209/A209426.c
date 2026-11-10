/*
 * A209426  Central terms of triangle A209424.
 * https://oeis.org/A209426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(2n, n), where T(r,c) = [x^r y^c] exp( Sum_{m>=1} x^m/m
 *            * Sum_{k=0..m} binomial(m,k)^m y^k ); T(0,0) = 1, T(r,c) = (1/r)
 *            * Sum_{m=1..r} Sum_{l=0..c} T(r-m,c-l) * binomial(m,l)^m
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-07)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t R = (size_t)(2 * n);
    size_t C = (size_t)n;
    size_t W = C + 1;

    seq_t *bp = mem_new(seq_t, (R + 1) * W);
    seq_t *T = mem_new(seq_t, (R + 1) * W);
    seq_t *temp = mem_new(seq_t, W);

    for (size_t m = 1; m <= R; ++m) {
        for (size_t l = 0; l <= m && l <= C; ++l) {
            seq_t b = binomial((idx_t)m, (idx_t)l);
            seq_t p = (seq_t)1;
            for (size_t e = 0; e < m; ++e) p = p * b;
            bp[m * W + l] = p;
        }
    }

    T[0] = (seq_t)1;
    for (size_t r = 1; r <= R; ++r) {
        for (size_t c = 0; c < W; ++c) temp[c] = (seq_t)0;
        for (size_t m = 1; m <= r; ++m) {
            size_t rem = r - m;
            for (size_t k = 0; k <= rem && k <= C; ++k) {
                seq_t t = T[rem * W + k];
                if (t == (seq_t)0) continue;
                for (size_t l = 0; l <= m && k + l <= C; ++l) {
                    temp[k + l] += t * bp[m * W + l];
                }
            }
        }
        for (size_t c = 0; c < W && c <= r; ++c)
            T[r * W + c] = temp[c] / (seq_t)r;
    }

    seq_t res = T[R * W + C];
    mem_free(bp);
    mem_free(T);
    mem_free(temp);
    return res;
}
