/*
 * A005729  a(n) is the smallest positive integer a for which there is an
 *          identity of the form a*n*x = Sum_{i=1..m} ai*gi(x)^n where a1,
 *          ..., am are in Z and g1(x), ..., gm(x) are in Z[x].
 * https://oeis.org/A005729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product of primes p with p^e(p), where e(p) = 1 if (p | n
 *            and n != p) or if n is divisible by some (p^(d*m)-1)/(p^d-1)
 *            with d >= 1, m >= 2; for p = 2, e(2) = 2 when 6 | n. Otherwise
 *            e(p) = 0
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 143, u64 n <= 359,
 *            u128 n <= 2519, big any n
 * Helpers:   a5729_isprime, a5729_repunit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a5729_isprime.h"
#include "../utils/a5729_repunit.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t nn = (uint64_t)n;
    uint64_t p;
    for (p = 2; p <= nn; p++) {
        if (!a5729_isprime(p)) continue;
        if ((nn % p == 0 && nn != p) || a5729_repunit(nn, p)) {
            r *= (seq_t)p;
            if (p == 2 && nn % 6 == 0)
                r *= (seq_t)2;
        }
    }
    return r;
}
