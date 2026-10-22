/*
 * A003972  Moebius transform of A003961; a(n) = phi(A003961(n)), where
 *          A003961 shifts the prime factorization of n one step towards the
 *          larger primes.
 * https://oeis.org/A003972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Multiplicative with a(p^e) = (q-1)*q^(e-1) where q =
 *            nextPrime(p)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9496, u64 n <= 9496,
 *            u128 n <= 9496, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"


static uint64_t next_prime(uint64_t p) {
    if (p == 2) return 3;
    uint64_t q = p + 2;
    while (!is_prime(q)) {
        q += 2;
    }
    return q;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            e++;
            m /= 2;
        }
        seq_t term = 2;
        for (uint64_t i = 1; i < e; i++) {
            term *= 3;
        }
        result *= term;
    }
    
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            
            uint64_t q = next_prime(p);
            seq_t term = (seq_t)(q - 1);
            for (uint64_t i = 1; i < e; i++) {
                term *= (seq_t)q;
            }
            result *= term;
        }
    }
    
    if (m > 1) {
        uint64_t q = next_prime(m);
        result *= (seq_t)(q - 1);
    }
    
    return result;
}
