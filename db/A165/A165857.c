/*
 * A165857  Totally multiplicative sequence with a(p) = 36.
 * https://oeis.org/A165857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 36^Ω(n) where Ω(n) is the total number of prime factors
 *            (with multiplicity)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* For n < 1 the offset does not matter – return 1. */
    if (n == 0) return (seq_t)1;
    /* Compute Ω(n). */
    seq_t omega = bigomega(n);
    /* Convert the exponent to a machine integer for bit‑shifting. */
    idx_t exp = (idx_t)omega;
    /* Binary exponentiation of 36^exp. */
    seq_t base = (seq_t)36;
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
