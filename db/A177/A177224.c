/*
 * A177224  a(n) is the least prime of the form ((2n+1)^k - (2n-1)^k)/2.
 * https://oeis.org/A177224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = the least prime of the form ((2n+1)^k - (2n-1)^k)/2 where
 *            k is an odd prime. The implementation tests odd primes k in
 *            ascending order until the corresponding value is prime
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* n is at least 1 (OEIS offset 1). */
    seq_t base1 = ((seq_t)2 * (seq_t)n) + 1;   /* 2n + 1  */
    seq_t base2 = ((seq_t)2 * (seq_t)n) - 1;   /* 2n - 1  */

    uint64_t k = 3;                            /* first odd prime */
    while (1) {
        seq_t p1 = seq_pow(base1, (idx_t)k);
        seq_t p2 = seq_pow(base2, (idx_t)k);
        seq_t val = (p1 - p2) >> 1;            /* (a^k - b^k) / 2 */

        if (is_prime_big(val))
            return val;

        k = next_prime(k + 1);                 /* next odd prime */
    }
}
