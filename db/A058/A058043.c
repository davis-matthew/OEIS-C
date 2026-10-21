/*
 * A058043  a(n) = nextprime(n^2) - prevprime(n^2).
 * https://oeis.org/A058043
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(n^2) - prevprime(n^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A058043.  For n < 2 the result is undefined and
   the function simply returns 0. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 2 only. */
    if (n < 2) return 0;

    uint64_t m = (uint64_t)n;
    uint64_t m2 = m * m;

    /* Find the largest prime less than m2. */
    uint64_t prev = m2 - 1;
    while (prev >= 2 && !is_prime(prev)) {
        --prev;
    }

    /* Find the smallest prime greater than m2. */
    uint64_t next = m2 + 1;
    while (!is_prime(next)) {
        ++next;
    }

    /* The difference always fits into seq_t for the intended range. */
    return (seq_t)(next - prev);
}
