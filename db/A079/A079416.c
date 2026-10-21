/*
 * A079416  a(n) = round(prime(n)/n).
 * https://oeis.org/A079416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(prime(n)/n)
 * Range:     u8 n <= 54, u16 n <= 6594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* n is assumed >= 1; behavior for n==0 is undefined */
    /* Ensure we have at least n primes available */
    if ((size_t)n > primes_cnt) {
        idx_t needed_limit = _prime_limit_for_idx(n);
        if (primes_limit < needed_limit) {
            /* free the old table if any */
            free(primes);
            primes = NULL;
            build_primes_upto(needed_limit, &primes, &primes_cnt);
            primes_limit = needed_limit;
        }
    }

    /* Retrieve the n‑th prime (array is 0‑based) */
    seq_t prime_n = (seq_t)primes[n - 1];

    /* Compute round(prime_n / n) without floating point */
    seq_t q = prime_n / (seq_t)n;
    seq_t r = prime_n % (seq_t)n;

    if (r * 2 >= (seq_t)n) {
        ++q;
    }

    return q;
}
