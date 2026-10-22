/*
 * A001358  Semiprimes (or biprimes): products of two primes.
 * https://oeis.org/A001358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = n-th semiprime, where semiprimes are numbers with exactly
 *            2 prime factors (counting multiplicity)
 * Range:     u8 n <= 82, u16 n <= 8380, u32 n <= 8380, u64 n <= 8380,
 *            u128 n <= 8380, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    seq_t found = 0;
    
    for (seq_t candidate = 2; ; candidate++) {
        if (count_prime_factors(candidate) == 2) {
            found++;
            if (found == (seq_t)n) return candidate;
        }
    }
}
