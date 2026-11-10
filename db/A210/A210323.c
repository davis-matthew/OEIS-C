/*
 * A210323  Number of 2-divided words of length n over a 3-letter alphabet.
 * https://oeis.org/A210323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n - A001867(n), where A001867(n) = (1/n)*Sum_{d|n}
 *            phi(d)*3^(n/d)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

static seq_t A001867_a(idx_t n) {
    if (n == 0) return 1;
    seq_t Q = 0;
    uint64_t R = 0;
    idx_t d;
    for (d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        idx_t e = n / d, i;
        seq_t q = (seq_t)(1 / n);
        uint64_t r = 1 % n;
        for (i = 0; i < e; i++) {
            uint64_t t = r * 3;
            q = q * 3 + (seq_t)(t / n);
            r = t % n;
        }
        uint64_t ph = totient_u64(d);
        Q += q * (seq_t)ph;
        R += ph * r;
        Q += (seq_t)(R / n);
        R %= n;
    }
    return Q;
}

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++) p = p * 3;
    return p - A001867_a(n);
}
