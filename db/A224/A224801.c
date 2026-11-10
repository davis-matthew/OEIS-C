/*
 * A224801  Column 1 of triangle A224798.
 * https://oeis.org/A224801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n+1,1), where T(i,i)=1, T(i,i-1)=2^(i-1), and for
 *            0<=j<=i-2, T(i,j) = 2*T(i,j+1) + Sum_{k=j+2..i-1}
 *            T(i,k)*T(k,j+1) (A224798)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t m = (size_t)n + 1;
    size_t total = (m + 1) * (m + 2) / 2;
    seq_t *T = mem_new(seq_t, total);
    seq_t pw = 1; /* 2^(i-1) */
    for (size_t i = 1; i <= m; ++i) {
        size_t base = i * (i + 1) / 2;
        T[base + i] = (seq_t)1;
        T[base + i - 1] = pw;
        pw = pw * (seq_t)2;
        for (size_t j = i; j-- > 1; ) {
            if (j + 2 > i) continue;
            size_t jj = j - 0;
            if (jj > i - 2) continue;
            seq_t sum = 0;
            for (size_t k = jj + 2; k < i; ++k) {
                sum += T[base + k] * T[k * (k + 1) / 2 + jj + 1];
            }
            T[base + jj] = (seq_t)2 * T[base + jj + 1] + sum;
        }
    }
    seq_t r = T[m * (m + 1) / 2 + 1];
    mem_free(T);
    return r;
}
