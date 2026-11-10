/*
 * A224607  a(n) = A219331(n^2).
 * https://oeis.org/A224607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A219331(n^2), where A219331 is the logarithmic derivative
 *            of A006456: c(m) = m*B(m) - Sum_{k=1..m-1} c(k)*B(m-k), B =
 *            A006456, B(0) = 1, B(m) = Sum_{1<=k^2<=m} B(m-k^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t N = n * n;
    seq_t *B = mem_new(seq_t, N + 1);
    seq_t *c = mem_new(seq_t, N + 1);
    B[0] = 1;
    for (idx_t i = 1; i <= N; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k * k <= i; k++) s += B[i - k * k];
        B[i] = s;
    }
    for (idx_t m = 1; m <= N; m++) {
        seq_t s = (seq_t)m * B[m];
        for (idx_t k = 1; k < m; k++) s -= c[k] * B[m - k];
        c[m] = s;
    }
    seq_t r = c[N];
    mem_free(B);
    mem_free(c);
    return r;
}
