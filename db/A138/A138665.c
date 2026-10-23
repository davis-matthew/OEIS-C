/*
 * A138665  Primes of the form 2*p(k)+3*p(k+1)+4*p(k+2) for some k, where
 *          p(k)=A000040(k).
 * https://oeis.org/A138665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2*p(k)+3*p(k+1)+4*p(k+2),
 *            where p(k) is the k-th prime
 * Range:     u8 n <= 1, u16 n <= 138, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state to avoid recomputing primes on every call */
    static uint64_t *primes = NULL;
    static size_t   primes_cnt = 0;
    static idx_t    prime_limit = 0;

    size_t found = 0;
    idx_t  k = 0;

    while (found < n) {
        /* Ensure we have primes up to index k+2 */
        while ((idx_t)primes_cnt <= k + 2) {
            idx_t new_limit = (prime_limit == 0) ? 1000 : prime_limit * 2;
            free(primes);                      /* free previous array */
            build_primes_upto(new_limit, &primes, &primes_cnt);
            prime_limit = new_limit;
        }

        seq_t val =
            (seq_t)2 * (seq_t)primes[k] +
            (seq_t)3 * (seq_t)primes[k + 1] +
            (seq_t)4 * (seq_t)primes[k + 2];

        if (is_prime_big(val)) {
            ++found;
            if (found == n) {
                return val;
            }
        }
        ++k;
    }

    /* Unreachable, but required to silence all compilers */
    return (seq_t)0;
}
