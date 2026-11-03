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
 * Validated: standard, pass, 76 terms through n = 9992 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* undefined below offset */

    /* Find the (n-3)th composite number */
    idx_t target = n - 3;                /* number of composites to skip */
    uint64_t count = 0;
    uint64_t cand = 4;
    while (count < target) {
        if (!is_prime_u64(cand)) ++count;
        if (count < target) ++cand;
    }
    uint64_t comp = cand;

    /* Find nearest prime < comp */
    uint64_t prev = comp - 1;
    while (!is_prime_u64(prev)) --prev;

    /* Find nearest prime > comp */
    uint64_t next = comp + 1;
    while (!is_prime_u64(next)) ++next;

    return (seq_t)prev + (seq_t)next;
}
