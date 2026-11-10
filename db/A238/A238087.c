/*
 * A238087  A129779(n+2)/A006954(n).
 * https://oeis.org/A238087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -A004193(n+1)/A000367(n+1) for n >= 2, i.e. with m = n-1,
 *            a(n) = (-1)^m * 2*(2m+1)!! / D(m), where D(m) = product of
 *            primes p with (p-1) | 2m (von Staudt-Clausen); a(0) = -1, a(1) =
 *            1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 31,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 200 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    uint64_t m = (uint64_t)n - 1;
    uint64_t b = 2 * m;
    size_t cnt = 0, cap = 64;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t d = 1; d * d <= b; d++) {
        if (b % d == 0) {
            uint64_t e[2];
            e[0] = d;
            e[1] = b / d;
            for (int t = 0; t < 2; t++) {
                if (t == 1 && e[1] == e[0]) break;
                uint64_t p = e[t] + 1;
                if (p != 2 && is_prime_u64(p)) {
                    if (cnt == cap) {
                        cap *= 2;
                        pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
                    }
                    pr[cnt++] = p;
                }
            }
        }
    }
    seq_t res = (seq_t)1;
    for (uint64_t f = 3; f <= 2 * m + 1; f += 2) {
        uint64_t g = f;
        for (size_t i = 0; i < cnt; i++) {
            if (pr[i] > 1 && g % pr[i] == 0) {
                g /= pr[i];
                pr[i] = 1;
            }
        }
        res *= (seq_t)g;
    }
    free(pr);
    if (m % 2 == 1) res = -res;
    return res;
}
