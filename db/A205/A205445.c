/*
 * A205445  s(A205446), where s(j)=Fibonacci(2j-1).
 * https://oeis.org/A205445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(j) where s(t)=Fibonacci(2t-1) and (k,j), j<k, is the
 *            pair with least k such that n divides s(k)-s(j); found by a
 *            residue search, with s(t)=3s(t-1)-s(t-2), s(1)=1, s(2)=2
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 24, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
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
    uint64_t m = (uint64_t)n;
    uint64_t *first = (uint64_t *)calloc((size_t)m, sizeof(uint64_t));
    if (!first) return (seq_t)0;

    uint64_t prev = 0, cur = 0;
    uint64_t jfound = 1;
    for (uint64_t t = 1; ; ++t) {
        uint64_t r;
        if (t == 1) r = 1 % m;
        else if (t == 2) r = 2 % m;
        else r = ((3 % m) * cur % m + m - prev) % m;
        if (first[r] != 0) {
            jfound = first[r];
            break;
        }
        first[r] = t;
        prev = cur;
        cur = r;
    }
    free(first);

    seq_t x = 1, y = 2;
    if (jfound == 1) return x;
    for (uint64_t t = 2; t < jfound; ++t) {
        seq_t z = 3 * y - x;
        x = y;
        y = z;
    }
    return y;
}
