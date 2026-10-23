/*
 * A024934  Sum of remainders n mod p, over all primes p < n.
 * https://oeis.org/A024934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p < n, p prime} n mod p
 * Range:     u8 n <= 78, u16 n <= 1582, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* No primes less than 2, so the sum is 0 for n = 0,1,2. */
    if (n <= 2)
        return (seq_t)0;

    size_t cnt = 0;
    uint64_t *primes = NULL;

    /* Build all primes less than n (i.e., ≤ n‑1). */
    build_primes_upto(n - 1, &primes, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)(n % (idx_t)primes[i]);

    /* The helper allocates the array with malloc; free it. */
    free(primes);

    return sum;
}
