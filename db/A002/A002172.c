/*
 * A002172  Glaisher's chi numbers chi(p) for p a prime of the form 4m+1.
 * https://oeis.org/A002172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, sign
 * Formula:   a(n) = 2*x where p = A002144(n) = x^2 + y^2 with y even and x +
 *            y == 1 (mod 4); p found by trial-division search
 * Range:     i8 n <= 283, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   a002172_isprime, a002172_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002172_isprime.h"
#include "../utils/a002172_isqrt.h"

seq_t a(idx_t n) {
    uint64_t count = 0, p = 1, x = 0, y = 0, i;
    seq_t r;
    while (count < (uint64_t)n) {
        p += 4;
        if (a002172_isprime(p)) count++;
    }
    for (i = 1; i * i < p; i += 2) {
        uint64_t rem = p - i * i;
        uint64_t j = a002172_isqrt(rem);
        if (j * j == rem) { x = i; y = j; break; }
    }
    r = (seq_t)2 * (seq_t)x;
    if ((x + y) % 4 != 1) r = -r;
    return r;
}
