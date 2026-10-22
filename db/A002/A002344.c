/*
 * A002344  Numbers x such that p = x^2 + 7y^2, with prime p = A033207(n).
 * https://oeis.org/A002344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: walk through the primes p in increasing order, keep
 *            those with p = x^2 + 7*y^2 (these are 7 and the primes p == 1,
 *            2, 4 mod 7); a(n) is the x >= 0 for the n-th such prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a2344_isprime, a2344_isqrt, a2344_rep
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2344_isprime.h"
#include "../utils/a2344_isqrt.h"
#include "../utils/a2344_rep.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p;
    (void)a2344_isqrt;
    for (p = 2;; p++) {
        uint64_t m = p % 7;
        uint64_t x;
        if (!(p == 7 || m == 1 || m == 2 || m == 4)) continue;
        if (!a2344_isprime(p)) continue;
        if (a2344_rep(p, &x)) {
            count++;
            if (count == n) return (seq_t)x;
        }
    }
}
