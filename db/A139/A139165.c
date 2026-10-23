/*
 * A139165  a(n) = prime(n)!/7 + 1.
 * https://oeis.org/A139165
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)!/7 + 1
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* For n < 4 the OEIS offset is undefined; return 0 in that case. */
    if (n < 4)
        return (seq_t)0;
    /* Get the nth prime (primes are 1‑based). */
    uint32_t p = get_prime(n);
    /* Compute p! exactly. */
    seq_t f = factorial((idx_t)p);
    /* Divide by 7 and add 1. */
    seq_t res = f / (seq_t)7 + (seq_t)1;
    return res;
}
