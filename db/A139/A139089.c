/*
 * A139089  a(n) = prime(n)!/9 + 1.
 * https://oeis.org/A139089
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)!/9 + 1
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 4) {
        /* Undefined for indices below the OEIS offset. */
        return (seq_t)0;
    }
    /* Obtain the n-th prime (1‑based). */
    uint32_t p = get_prime(n);
    /* Compute p! exactly.  factorial is supplied as a helper. */
    seq_t f = factorial((idx_t)p);
    /* Since p >= 7 for n >= 4, p! is divisible by 9. */
    return f / (seq_t)9 + (seq_t)1;
}
