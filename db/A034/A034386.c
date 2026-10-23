/*
 * A034386  Primorial numbers (second definition): n# = product of primes <=
 *          n.
 * https://oeis.org/A034386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = product of all primes p ≤ n (primorial)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* By definition the primorial of 0 and 1 is 1. */
    if (n <= 1)
        return (seq_t)1;

    size_t cnt;
    uint64_t *primes;
    /* Generate all primes ≤ n.  The helper allocates the array. */
    build_primes_upto(n, &primes, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        prod *= (seq_t)primes[i];

    /* Free the array allocated by build_primes_upto if desired.
       The helper may manage memory internally, so freeing is optional. */
    free(primes);

    return prod;
}
