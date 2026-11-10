/*
 * A234016  Partial sums of the characteristic function of A055938.
 * https://oeis.org/A234016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (A046699(n+2) - 1), where A046699(1) = A046699(2) =
 *            1, A046699(m) = A046699(m - A046699(m-1)) + A046699(m-1 -
 *            A046699(m-2)) for m > 2
 * Range:     u8 n <= 511, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 86 terms through n = 6638 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m = n + 2;
    seq_t *c = mem_new(seq_t, (size_t)m + 1);
    if (!c) return 0;
    c[1] = 1;
    c[2] = 1;
    for (idx_t i = 3; i <= m; ++i) {
        idx_t i1 = i - (idx_t)c[i - 1];
        idx_t i2 = (i - 1) - (idx_t)c[i - 2];
        c[i] = c[i1] + c[i2];
    }
    seq_t r = (seq_t)n - (c[m] - 1);
    mem_free(c);
    return r;
}
