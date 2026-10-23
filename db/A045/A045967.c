/*
 * A045967  a(1)=4; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+1}^{e_i+1}.
 * https://oeis.org/A045967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1)=4; for n>1, if n=∏ p_i^e_i then a(n)=∏ p_{i+1}^{e_i+1}
 * Range:     u8 n <= 9, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)4;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t next = next_prime(primes[i]);          /* successor prime */
        uint64_t exp = exps[i] + 1;                     /* exponent + 1 */
        seq_t term = 1;
        for (uint64_t j = 0; j < exp; ++j)
            term = term * (seq_t)next;
        result = result * term;
    }
    return result;
}
