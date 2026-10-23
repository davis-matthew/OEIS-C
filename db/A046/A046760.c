/*
 * A046760  Wasteful numbers.
 * https://oeis.org/A046760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A046760.
   Compute the n‑th wasteful number by brute force search.
   A number n is wasteful if, when written as a product of primes raised to
   powers, the total number of digits of the primes and (when >1) their
   exponents exceeds the number of decimal digits of n.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_count.h"

/* Return true iff x is a wasteful number */
static bool is_wasteful(uint64_t x)
{
    if (x < 2) return false;

    uint64_t m = x;
    uint64_t dsum = 0;

    /* handle factor 2 separately */
    if ((m & 1ULL) == 0) {
        unsigned exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++exp;
        }
        dsum += digit_count(2);
        if (exp > 1)
            dsum += digit_count(exp);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            dsum += digit_count(p);
            if (exp > 1)
                dsum += digit_count(exp);
        }
    }

    /* remaining prime factor > sqrt(original x) */
    if (m > 1)
        dsum += digit_count(m);

    uint64_t l = digit_count(x);
    return dsum > l;
}

/* Return the n-th wasteful number (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    uint64_t count = 0;
    uint64_t candidate = 0;

    while (count < n) {
        ++candidate;
        if (is_wasteful(candidate))
            ++count;
    }

    return (seq_t)candidate;
}
