/*
 * A187279  a(n) is the least number of terms needed to represent n as a sum
 *          of powers of the same prime.
 * https://oeis.org/A187279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{p prime} sum of digits of n when written in base p
 * Range:     u8 n <= 1441, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1, so n>=1 in use */

    static idx_t prime_limit = 0;
    static size_t prime_cnt   = 0;
    static uint64_t *primes   = NULL;

    if (primes == NULL || prime_limit < n) {   /* need primes up to n */
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t best = (seq_t)n;           /* trivial upper bound (base > n) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        seq_t sum = 0;
        idx_t t = n;
        while (t > 0) {
            sum += t % p;
            t /= p;
        }
        if (sum < best) best = sum;
        if (best == 1) break;        /* cannot beat 1 */
    }

    return best;
}
