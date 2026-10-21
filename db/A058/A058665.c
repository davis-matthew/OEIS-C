/*
 * A058665  a(n) = gcd(n+1, n-phi(n)).
 * https://oeis.org/A058665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n+1, n-phi(n))
 * Range:     u8 n <= 740, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* compute Euler's totient φ(n) */
    seq_t phi = (seq_t)n;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            phi -= phi / (seq_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        phi -= phi / (seq_t)tmp;
    }
    seq_t s = (seq_t)n + 1;     /* n+1 */
    seq_t diff = (seq_t)n - phi; /* n - φ(n) */
    return gcd(s, diff);
}
