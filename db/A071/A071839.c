/*
 * A071839  Number of primes == 1 mod 8 <= n.
 * https://oeis.org/A071839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Number of primes ≡ 1 (mod 8) ≤ n (OEIS A071839)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* n < 2 has no primes, so answer is 0 */
    if (n < 2) return 0;

    uint64_t *primes = NULL;
    size_t   cnt     = 0;

    /* generate all primes ≤ n */
    build_primes_upto(n, &primes, &cnt);

    uint64_t count = 0;
    for (size_t i = 0; i < cnt; ++i)
        if ((primes[i] & 7U) == 1)   /* equivalent to primes[i] % 8 == 1 */
            ++count;

    /* free allocated memory for the primes list */
    free(primes);

    return (seq_t)count;
}
