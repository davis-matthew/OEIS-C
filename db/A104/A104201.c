/*
 * A104201  Sums of straddle primes.
 * https://oeis.org/A104201
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the nearest primes preceding and following the
 *            (n-3)th composite number (indices start at 4)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/my_is_prime.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* undefined below offset */

    /* Find the (n-3)th composite number */
    idx_t target = n - 3;                /* number of composites to skip */
    uint64_t count = 0;
    uint64_t cand = 4;
    while (count < target) {
        if (!my_is_prime(cand)) ++count;
        if (count < target) ++cand;
    }
    uint64_t comp = cand;

    /* Find nearest prime < comp */
    uint64_t prev = comp - 1;
    while (!my_is_prime(prev)) --prev;

    /* Find nearest prime > comp */
    uint64_t next = comp + 1;
    while (!my_is_prime(next)) ++next;

    return (seq_t)prev + (seq_t)next;
}
