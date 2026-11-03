/*
 * A115396  Smallest prime p such that p +- 2*n are semiprimes.
 * https://oeis.org/A115396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p – 2*n and p + 2*n are
 *            semiprimes. The function searches successive primes starting
 *            from the first prime ≥ 4 + 2*n until both adjusted numbers have
 *            exactly two prime factors (counted with multiplicity)
 * Range:     u8 n <= 41, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   bigomega_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* n is at least 1 (offset 1). */
    uint64_t start = 4 + 2 * (uint64_t)n;           /* p – 2*n >= 4  ⇒  p >= 4+2*n */
    uint64_t p = next_prime(start - 1);              /* first prime ≥ start */
    for (;;) {
        seq_t a1 = (seq_t)p - (seq_t)2 * n;
        seq_t a2 = (seq_t)p + (seq_t)2 * n;
        /* both values must be ≥ 4 to be semiprime */
        if (a1 >= 4 && a2 >= 4) {
            if (bigomega_seq(a1) == 2 && bigomega_seq(a2) == 2)
                return (seq_t)p;
        }
        p = next_prime(p + 1);                       /* next candidate prime */
    }
}
