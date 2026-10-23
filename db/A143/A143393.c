/*
 * A143393  a(n) is the least prime p such that p*(p+prime(n))-1 is prime with
 *          prime(i)=i-th prime.
 * https://oeis.org/A143393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p*(p+prime(n)) − 1 is prime.
 *            Computed by iterating over primes and testing each candidate
 * Range:     u8 n <= 11, u16 n <= 1142, u32 n <= 1142, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset */
    uint32_t prime_n = get_prime((idx_t)n); /* nth prime (1‑based) */
    size_t i = 1;
    while (1) {
        seq_t p = (seq_t)get_prime((idx_t)i);
        seq_t prod = p * (p + (seq_t)prime_n) - 1;
        if (is_prime_big(prod))
            return p;
        ++i;
    }
}
