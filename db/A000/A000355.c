/*
 * A000355  Primes p == 3, 9, 11 (mod 20) such that 2p+1 is also prime.
 * https://oeis.org/A000355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes p == 3, 9, 11 (mod 20) such that 2p+1 is also prime
 * Range:     u8 n <= 7, u16 n <= 301, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime(seq_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    
    for (seq_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 2;
    
    while (true) {
        if (is_prime(candidate)) {
            seq_t mod = candidate % 20;
            if ((mod == 3 || mod == 9 || mod == 11) && 
                is_prime(2 * candidate + 1)) {
                count++;
                if (count == n) {
                    return candidate;
                }
            }
        }
        candidate++;
    }
}
