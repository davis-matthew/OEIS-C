/*
 * A000729  Expansion of Product_{k >= 1} (1 - x^k)^6.
 * https://oeis.org/A000729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(4*n+1), b multiplicative with b(2^e) = 0^e, b(p^e) =
 *            p^e*(1+(-1)^e)/2 if p == 3 (mod 4), b(p^e) = b(p)*b(p^(e-1)) -
 *            p^2*b(p^(e-2)) if p == 1 (mod 4), b(1) = 1, b(p) = 2*(x^2 - y^2)
 *            where p = x^2 + y^2 and y is even
 * Range:     i8 n <= 20, i16 n <= 2145, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   a729_bp, a729_pe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a729_bp.h"
#include "../utils/a729_pe.h"

seq_t a(idx_t n) {
    long long m = 4 * (long long)n + 1;
    long long result = 1;
    long long t = m;
    long long p;
    for (p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            long long e = 0;
            while (t % p == 0) { e++; t /= p; }
            {
                long long f = a729_pe(p, e);
                if (f == 0) return (seq_t)0;
                result *= f;
            }
        }
    }
    if (t > 1) {
        long long f = a729_pe(t, 1);
        if (f == 0) return (seq_t)0;
        result *= f;
    }
    return (seq_t)result;
}
