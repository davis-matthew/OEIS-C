/*
 * A005382  Primes p such that 2p-1 is also prime.
 * https://oeis.org/A005382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime p such that 2p-1 is also prime */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 2;
    
    while (1) {
        if (is_prime((uint64_t)candidate)) {
            seq_t two_p_minus_1 = 2 * candidate - 1;
            if (is_prime((uint64_t)two_p_minus_1)) {
                count++;
                if (count == n) {
                    return candidate;
                }
            }
        }
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }
}
