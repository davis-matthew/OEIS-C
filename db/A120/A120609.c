/*
 * A120609  Primes among the absolute value of numbers of the form f(x)= x^2 +
 *          x - 1354363.
 * https://oeis.org/A120609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120609: the n‑th prime among the absolute values of
   f(x) = x^2 + x - 1354363 (x >= 1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) abort();                /* undefined for 0 based on specification */

    idx_t count = 0;
    const seq_t offset = (seq_t)1354363; /* constant part of the quadratic */

    for (idx_t x = 1; ; ++x) {
        /* Compute x*(x+1) using the seq_t type directly. */
        seq_t prod = (seq_t)x * ((seq_t)x + (seq_t)1);

        /* Take the absolute difference from the offset. */
        seq_t val;
        if (prod >= offset)
            val = prod - offset;
        else
            val = offset - prod;

        /* Zero cannot occur for integer x here, but test just in case. */
        if (val == 0) continue;

        /* Check primality. */
        if (is_prime_big(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
