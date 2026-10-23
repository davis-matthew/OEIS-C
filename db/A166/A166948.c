/*
 * A166948  The count of smallest prime factors in n-th composite.
 * https://oeis.org/A166948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the smallest prime factor of the n‑th
 *            composite number
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation iterates through integers starting from 4,
    counts composites until the n‑th one is found, then uses
    simple trial division to determine the exponent of its
    smallest prime factor.  It relies only on the provided
    is_prime function and the standard C types. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* guard against n = 0 (undefined for the sequence) */
    if (n == 0) return 0;

    idx_t count = 0;
    idx_t candidate = 4;          /* first composite number */

    /* Find the n-th composite number */
    while (count < n) {
        if (!is_prime(candidate)) {
            count++;
            if (count == n) {
                break;
            }
        }
        candidate++;
    }

    /* candidate now holds the n-th composite number.  Determine
       the exponent of its smallest prime factor. */
    uint64_t temp = (uint64_t)candidate;
    seq_t exp = 0;
    uint64_t p = 0;

    if (temp % 2 == 0) {
        p = 2;
    } else {
        /* trial division for odd factors */
        for (uint64_t i = 3; i * i <= temp; i += 2) {
            if (temp % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0) {
            /* temp itself is prime, but then candidate would be prime;
               the only case is when candidate == 2 (not composite).
               For completeness, set p = temp. */
            p = temp;
        }
    }

    /* Count multiplicity of p in temp */
    while (temp % p == 0) {
        temp /= p;
        exp++;
    }

    return exp;
}
