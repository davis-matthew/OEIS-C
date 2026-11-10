/*
 * A071016  Stirling_2 transform of A002487.
 * https://oeis.org/A071016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Stirling2(n,k) * A002487(k+1), with
 *            A002487(0)=0, A002487(1)=1, A002487(2m)=A002487(m),
 *            A002487(2m+1)=A002487(m)+A002487(m+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t A002487_a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n % 2 == 0) return A002487_a(n / 2);
    return A002487_a(n / 2) + A002487_a(n / 2 + 1);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t m = (size_t)n;
    seq_t *row = mem_new(seq_t, m + 2);
    size_t i, k;
    row[0] = 1; /* S2(0,0) */
    for (i = 1; i <= m; i++) {
        for (k = i; k >= 1; k--) {
            row[k] = row[k] * (seq_t)k + row[k - 1];
        }
        row[0] = 0;
    }
    seq_t sum = 0;
    for (k = 1; k <= m; k++) {
        sum += row[k] * A002487_a((idx_t)(k + 1));
    }
    mem_free(row);
    return sum;
}
