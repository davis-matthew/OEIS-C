/*
 * A059731  Row sums in A059922.
 * https://oeis.org/A059731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k) where T(m,k) = T(m-1,k-1)*T(m-1,k) +
 *            1, T(m,0) = T(m,m) = 1 (A059922 triangle)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *r = mem_new(seq_t, (N + 1));
    r[0] = 1;
    for (size_t i = 1; i <= N; ++i)
    {
        for (size_t j = i - 1; j >= 1; --j)
            r[j] = r[j - 1] * r[j] + 1;
        r[i] = 1;
    }
    seq_t s = 0;
    for (size_t j = 0; j <= N; ++j)
        s += r[j];
    mem_free(r);
    return s;
}
