/*
 * A056332  Number of primitive (aperiodic) reversible string structures with
 *          n beads using a maximum of three different colors.
 * https://oeis.org/A056332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d)*A001998(n/d-1), with A001998(k) =
 *            4*A001998(k-1) - 12*A001998(k-3) + 9*A001998(k-4), A001998(0)=1,
 *            A001998(1)=2, A001998(2)=4, A001998(3)=10
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int A056332_mu(idx_t m) {
    int r = 1;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t *c = mem_new(seq_t, (size_t)n + 4);
    c[0] = 1; c[1] = 2; c[2] = 4; c[3] = 10;
    for (idx_t i = 4; i < n; i++) {
        c[i] = (4 * c[i - 1] + 9 * c[i - 4]) - 12 * c[i - 3];
    }
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int m = A056332_mu(d);
        if (m == 1) pos += c[n / d - 1];
        else if (m == -1) neg += c[n / d - 1];
    }
    mem_free(c);
    return pos - neg;
}
