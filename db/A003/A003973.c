/*
 * A003973  Inverse Möbius transform of A003961; a(n) = sigma(A003961(n)),
 *          where A003961 shifts the prime factorization of n one step towards
 *          the larger primes.
 * https://oeis.org/A003973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = (q^(e+1)-1)/(q-1) where q =
 *            nextPrime(p)
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   geometric_sum, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/geometric_sum.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; p++) {
        idx_t e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            uint64_t q = next_prime(p);
            seq_t factor = geometric_sum((seq_t)q, e);
            result = result * factor;
        }
    }
    
    if (temp > 1) {
        uint64_t q = next_prime(temp);
        result = result * (1 + (seq_t)q);
    }
    
    return result;
}
