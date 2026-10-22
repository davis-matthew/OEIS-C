/*
 * A001916  Primes p such that the congruence 2^x = 5 (mod p) is solvable.
 * https://oeis.org/A001916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that 2^x ≡ 5 (mod p) has a solution
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   solvable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/solvable.h"

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (true) {
        if (is_prime(p) && solvable(p)) {
            count++;
            if (count == n) return (seq_t)p;
        }
        p++;
    }
}
