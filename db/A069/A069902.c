/*
 * A069902  Largest prime factor of n(n+1)/2, the n-th triangular number.
 * https://oeis.org/A069902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of n*(n+1)/2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

/* Return the largest prime factor of the n-th triangular number. */
seq_t a(idx_t n) {
    /* Ensure we work with the exact triangular number factorization.
       The product n*(n+1) is always even, so divide one factor by 2. */
    uint64_t part1, part2;
    if (n == 0) return 1;   /* Not used for the offset, but safe. */

    if (n & 1) {            /* n is odd: (n+1)/2 is even */
        part1 = (uint64_t)n;
        part2 = ((uint64_t)n + 1) / 2;
    } else {                /* n is even: n/2 is even */
        part1 = (uint64_t)n / 2;
        part2 = (uint64_t)n + 1;
    }

    /* Compute the largest prime factor of each part. */
    seq_t p1 = largest_prime_factor(part1);
    seq_t p2 = largest_prime_factor(part2);

    /* Compare the two and return the larger. */
    unsigned long long v1 = (unsigned long long)p1;
    unsigned long long v2 = (unsigned long long)p2;
    return (v1 >= v2) ? p1 : p2;
}
