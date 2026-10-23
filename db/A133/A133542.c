/*
 * A133542  Sum of sixth powers of five consecutive primes.
 * https://oeis.org/A133542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{4} p_{n+k}^6, where p_m denotes the m‑th prime
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum = 0;
    for (idx_t k = 0; k < 5; ++k) {
        seq_t p = (seq_t)get_prime(n + k);
        sum += pow_int(p, (idx_t)6);
    }
    return sum;
}
