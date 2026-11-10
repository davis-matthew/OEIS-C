/*
 * A055840  T(2n+6,n), where T is the array in A055830.
 * https://oeis.org/A055840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(2n+6, n), where T is the triangle of A055830: T(i,0) =
 *            Fibonacci(i+1); T(i,j) = 0 for j >= i; T(i,j) = T(i-1,j-1) +
 *            T(i-1,j) + T(i-2,j) for 1 <= j < i
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 489 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t R = 2 * N + 6;
    seq_t *prev = mem_new(seq_t, R + 1);
    seq_t *cur  = mem_new(seq_t, R + 1);
    if (!prev || !cur) exit(1);

    /* column 0: Fib(i+1) */
    {
        seq_t f0 = 1, f1 = 1;   /* Fib(1), Fib(2) */
        prev[0] = f0;
        if (R >= 1) prev[1] = f1;
        for (size_t i = 2; i <= R; ++i) {
            seq_t f2 = f0 + f1;
            prev[i] = f2;
            f0 = f1;
            f1 = f2;
        }
    }

    for (size_t j = 1; j <= N; ++j) {
        for (size_t i = 0; i <= j && i <= R; ++i) cur[i] = 0;
        for (size_t i = j + 1; i <= R; ++i) {
            seq_t v = prev[i - 1] + cur[i - 1];
            if (i >= 2) v = v + cur[i - 2];
            cur[i] = v;
        }
        seq_t *t = prev; prev = cur; cur = t;
    }

    seq_t res = prev[R];
    mem_free(prev);
    mem_free(cur);
    return res;
}
