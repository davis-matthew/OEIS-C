/*
 * A001031  Goldbach conjecture: a(n) = number of decompositions of 2n into
 *          sum of two primes (counting 1 as a prime).
 * https://oeis.org/A001031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   count pairs (p, q) where p + q = 2n, 1 <= p <= q, both p and q
 *            are prime (1 counted as prime)
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_or_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_or_one.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t two_n = nn * 2;
    seq_t count = 0;
    
    for (seq_t i = 1; i <= nn; i++) {
        if (is_prime_or_one(i) && is_prime_or_one(two_n - i)) {
            count++;
        }
    }
    
    return count;
}
