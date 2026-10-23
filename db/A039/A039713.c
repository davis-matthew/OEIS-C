/*
 * A039713  a(n) = n-th prime modulo 15.
 * https://oeis.org/A039713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime modulo 15
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; behaviour for n == 0 is undefined,
       but we guard against it anyway. */
    if (n == 0) {
        return 0;
    }

    idx_t count = 0;          /* number of primes found so far   */
    idx_t candidate = 2;      /* current number to test          */
    idx_t prime = 0;          /* the n‑th prime once found      */

    while (true) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                prime = candidate;
                break;
            }
        }
        ++candidate;
    }

    return (seq_t)(prime % 15);
}
