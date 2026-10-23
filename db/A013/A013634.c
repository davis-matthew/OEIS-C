/*
 * A013634  a(n) = nextprime(n) + n.
 * https://oeis.org/A013634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(n) + n
 * Range:     u8 n <= 126, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Compute the smallest prime strictly greater than n */
    uint64_t start = (uint64_t)n + 1ULL;   /* n+1, possibly equal to 0 for overflow */
    uint64_t cand = start;

    /* Handle small values directly */
    if (cand <= 2ULL) {
        cand = 2ULL;
    } else if ((cand & 1ULL) == 0ULL) {   /* make it odd if even */
        cand++;
    }

    /* Search for the next prime (greater or equal to start) */
    while (!is_prime_small(cand)) {
        cand += 2ULL;  /* skip even numbers */
    }

    /* Return n + cand as a seq_t */
    return (seq_t)n + (seq_t)cand;
}
