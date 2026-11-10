/*
 * A091075  Primes occurring in A091074.
 * https://oeis.org/A091075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among b(k), where b(k) = b(k-1) + b(k-2), b(1)
 *            = 12, b(2) = 67 (A091074); found by scanning b(k) with a
 *            primality test
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    seq_t x = 12, y = 67;
    idx_t count = 0;
    for (;;) {
        if (is_prime_seq(x)) {
            count++;
            if (count == n) return x;
        }
        seq_t z = x + y;
        x = y;
        y = z;
    }
}
