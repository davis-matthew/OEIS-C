/*
 * A003626  Inert rational primes in Q(sqrt(-5)).
 * https://oeis.org/A003626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search over primes p with p mod 20 in {11, 13, 17, 19}; a(n) =
 *            n-th such prime
 * Range:     u8 n <= 28, u16 n <= 3301, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a003626_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a003626_isprime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 9;

    while (count < n) {
        uint64_t r;
        candidate += 2;
        r = candidate % 20;
        if ((r == 11 || r == 13 || r == 17 || r == 19) && a003626_isprime(candidate)) {
            count++;
        }
    }

    return (seq_t)candidate;
}
