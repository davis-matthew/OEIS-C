/*
 * A122102  a(n) = Sum_{k=1..n} prime(k)^4.
 * https://oeis.org/A122102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* safety, offset is 1 */

    /* Find an upper bound for the nth prime */
    idx_t limit = _prime_limit_for_idx(n);

    /* Generate all primes up to that limit.  The array will be of type
       uint64_t*, as required by build_primes_upto. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Sum the fourth powers.  We use a 128‑bit accumulator to avoid
       intermediate overflow on 64‑bit builds. */
    __uint128_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        __uint128_t p = primes[i];
        __uint128_t p2 = p * p;      /* p^2 */
        __uint128_t p4 = p2 * p2;    /* p^4 */
        sum += p4;
    }

    free(primes);
    return (seq_t)sum;
}
