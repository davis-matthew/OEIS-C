/*
 * A007280  Multiplicative encoding of partition triangle.
 * https://oeis.org/A007280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} prime(k)^A008284(n,k), where
 *            A008284(n,k) = T(n,k) counts partitions of n into exactly k
 *            parts: T(i,j) = T(i-1,j-1) + T(i-j,j), T(i,1) = T(i,i) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   ipow_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_u64.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t w = n + 1;
    idx_t i, j, k;
    uint64_t *T;
    uint64_t cand;
    seq_t result = 1;

    T = (uint64_t *)calloc((size_t)(w * w), sizeof(uint64_t));
    if (!T) return 0;
#define TT(i, j) T[(i) * w + (j)]
    TT(0, 0) = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i; j++) {
            uint64_t v = TT(i - 1, j - 1);
            if (i >= j) v += TT(i - j, j);
            TT(i, j) = v;
        }
    }

    cand = 1;
    for (k = 1; k <= n; k++) {
        do { cand++; } while (!is_prime_u64(cand));
        result *= ipow_u64((seq_t)cand, TT(n, k));
    }
#undef TT
    free(T);
    return result;
}
