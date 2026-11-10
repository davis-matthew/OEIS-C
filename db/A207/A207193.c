/*
 * A207193  Auxiliary function for computing the Carmichael lambda function
 *          (A002322).
 * https://oeis.org/A207193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(A000961(n)), where f(1) = 1, f(2^e) = 2^(e-2) if e > 2,
 *            and f(p^e) = (p-1)*p^(e-1) otherwise; A000961(n) is found by
 *            enumerating prime powers in increasing order
 * Range:     u8 n <= 71, u16 n <= 6636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9205 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a207193_pp(uint64_t m, uint64_t *p_out, unsigned *e_out) {
    uint64_t p = 0;
    if (m % 2 == 0) p = 2;
    else {
        for (uint64_t d = 3; d * d <= m; d += 2) {
            if (m % d == 0) { p = d; break; }
        }
        if (p == 0) p = m;
    }
    unsigned e = 0;
    while (m % p == 0) { m /= p; e++; }
    if (m != 1) return false;
    *p_out = p;
    *e_out = e;
    return true;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m = 0;
    uint64_t p = 1;
    unsigned e = 0;
    for (m = 1;; m++) {
        if (m == 1) {
            count++;
            if (count == (uint64_t)n) return (seq_t)1;
            continue;
        }
        if (a207193_pp(m, &p, &e)) {
            count++;
            if (count == (uint64_t)n) break;
        }
    }
    if (p == 2) {
        if (e > 2) return (seq_t)(m / 4);
        return (seq_t)(m / 2);
    }
    return (seq_t)(m / p) * (seq_t)(p - 1);
}
