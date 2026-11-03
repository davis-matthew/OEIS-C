/*
 * A033273  Number of nonprime divisors of n.
 * https://oeis.org/A033273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) - A001221(n) (number of nonprime divisors of
 *            n)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 120 terms through n = 8814 (2026-11-01)
 * Helpers:   num_divisors, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* OEIS indexing starts at n = 1.  For safety handle n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* divisor_count_u64 counts all (prime and composite) divisors of n;
       count_distinct_primes counts the distinct prime factors of n. */
    uint64_t d = num_divisors((uint64_t)n);
    int w = omega((uint64_t)n);

    return (seq_t)(d - (uint64_t)w);
}
