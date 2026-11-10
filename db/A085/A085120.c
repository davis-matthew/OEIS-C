/*
 * A085120  a(n) = Sum_{m=1..24} (25-m)^n*A005875(m).
 * https://oeis.org/A085120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..24} (25-m)^n * A005875(m), where A005875(m) =
 *            number of integer triples (i,j,k) with i^2+j^2+k^2 = m
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A005875_small(int m) {
    int c = 0;
    for (int i = -4; i <= 4; i++)
        for (int j = -4; j <= 4; j++)
            for (int k = -4; k <= 4; k++)
                if (i * i + j * j + k * k == m) c++;
    return c;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (int m = 1; m <= 24; m++) {
        int r = A005875_small(m);
        if (r == 0) continue;
        seq_t p = 1;
        seq_t base = (seq_t)(25 - m);
        for (idx_t e = 0; e < n; e++) p *= base;
        total += p * (seq_t)r;
    }
    return total;
}
