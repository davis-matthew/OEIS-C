/*
 * A072015  Maxima when the mapping of A072010 is applied to n repeatedly.
 * https://oeis.org/A072015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max of the trajectory f(n), f(f(n)), ... where f =
 *            A072010. The maximum is taken over the iterates, not n itself;
 *            Floyd cycle detection finds where the trajectory starts to
 *            repeat
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A072015_f(seq_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    seq_t m = n;
    while (m % 2 == 0) {
        m /= 2;
        result *= 2;
    }
    for (seq_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t t = (p % 4 == 1) ? p + 2 : p - 2;
            while (m % p == 0) {
                m /= p;
                result *= t;
            }
        }
    }
    if (m > 1) {
        seq_t t = (m % 4 == 1) ? m + 2 : m - 2;
        result *= t;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t best = (seq_t)0;
    seq_t t = x, h = x;
    for (;;) {
        t = A072015_f(t);
        if (t > best) best = t;
        h = A072015_f(h);
        if (h > best) best = h;
        h = A072015_f(h);
        if (h > best) best = h;
        if (t == h) break;
    }
    seq_t c = A072015_f(t);
    if (c > best) best = c;
    while (c != t) {
        c = A072015_f(c);
        if (c > best) best = c;
    }
    return best;
}
