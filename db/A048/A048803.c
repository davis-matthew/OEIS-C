/*
 * A048803  a(n) = Product_{k=1..n} rad(k), where rad(n) is the product of
 *          distinct prime factors of n, cf. A007947.
 * https://oeis.org/A048803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = product_{p prime <= n} p^floor(n/p)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Lazily build the list of primes up to the largest n that has
       been requested so far.  The array is retained between calls
       for efficiency. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    if (n > primes_limit) {
        /* Rebuild primes up to the new limit.  build_primes_upto
           allocates the array and sets primes and primes_cnt. */
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;           /* all remaining primes are too big */
        idx_t exp = n / p;          /* floor(n/p) */
        if (exp == 0) continue;
        seq_t pow_val = big_pow((seq_t)p, (seq_t)exp);
        result *= pow_val;
    }
    return result;
}
