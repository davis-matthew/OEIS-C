/*
 * A227094  Binomial transform of A013999.
 * https://oeis.org/A227094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*A013999(k); A013999: b(0)=1, b(1)=1,
 *            b(2)=2, b(n) = (n+3)*b(n-1) - (2n+1)*b(n-2) + n*b(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *b   = mem_new(seq_t, (size_t)n + 3);
    seq_t *row = mem_new(seq_t, (size_t)n + 3);
    if (!b || !row) exit(1);

    b[0] = 1;
    b[1] = 1;
    b[2] = 2;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = (seq_t)(i + 3) * b[i - 1];
        seq_t t2 = (seq_t)(2 * i + 1) * b[i - 2];
        seq_t t3 = (seq_t)i * b[i - 3];
        b[i] = t1 + t3 - t2;
    }

    /* Pascal row n, built in place with additions only */
    row[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            row[j] += row[j - 1];
        }
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += row[k] * b[k];
    }

    mem_free(b);
    mem_free(row);
    return sum;
}
