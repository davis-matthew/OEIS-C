/*
 * A007317  Binomial transform of Catalan numbers.
 * https://oeis.org/A007317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   (n+2)*a(n+2) = (6n+4)*a(n+1) - 5n*a(n), a(1) = 1, a(2) = 2
 *            (arbitrary precision); a(1) = 1, a(n) = 1 + Sum_{i=1..n-1}
 *            a(i)*a(n-i) (fixed width, exact modulo 2^w since it only uses +
 *            and *)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 197 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (seq_is_big()) {
        seq_t x = 1, y = 2;
        idx_t m;
        for (m = 1; m + 2 <= n; m++) {
            seq_t z = ((seq_t)(6 * m + 4) * y - (seq_t)(5 * m) * x) / (seq_t)(m + 2);
            x = y;
            y = z;
        }
        return y;
    } else {
        seq_t *v = mem_new(seq_t, (size_t)(n + 1));
        seq_t r;
        idx_t m, i;
        v[1] = 1;
        for (m = 2; m <= n; m++) {
            seq_t s = 0;
            for (i = 1; 2 * i < m; i++) s += v[i] * v[m - i];
            s = s + s;
            if (m % 2 == 0) s += v[m / 2] * v[m / 2];
            v[m] = s + 1;
        }
        r = v[n];
        mem_free(v);
        return r;
    }
}
