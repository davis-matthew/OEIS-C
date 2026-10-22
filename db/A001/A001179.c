/*
 * A001179  Leonardo logarithm of n.
 * https://oeis.org/A001179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   iterate the Pisano period pi(m) = A001175(m), starting at m = n,
 *            until a fixed point is reached; that point is A235249(n) =
 *            24*5^(a(n)-1) for n > 1, so a(n) = 1 + (number of factors 5 in
 *            it/24); a(1) = 0. pi(m) = lcm over p^e || m of p^(e-1)*pi(p),
 *            with pi(p) found by iterating the Fibonacci numb...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1179_gcd, a1179_pisano, a1179_pisano_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1179_gcd.h"
#include "../utils/a1179_pisano.h"
#include "../utils/a1179_pisano_prime.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n, next, q;
    seq_t k = 0;
    for (;;) {
        next = a1179_pisano(m);
        if (next == m) break;
        m = next;
    }
    if (m == 1) return (seq_t)0;
    q = m / 24;
    k = 1;
    while (q % 5 == 0) { q /= 5; k++; }
    return k;
}
