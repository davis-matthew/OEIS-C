/*
 * A036959  Primes with digits (0,...,5) taken as base 6 and converted to base
 *          10.
 * https://oeis.org/A036959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036959: a(n) is the nth integer k such that
   treating the base‑6 representation of k as a base‑10 string
   yields a prime number. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Search sequentially for the n‑th qualifying integer. */
    idx_t cur   = 1;     /* candidate base‑10 integer */
    idx_t count = 0;     /* number of qualifying integers found */

    for (;; ++cur) {
        /* Convert cur to base‑6 digits.  Least significant first. */
        uint8_t digits[64];
        size_t d = 0;
        idx_t tmp = cur;
        do {
            digits[d++] = (uint8_t)(tmp % 6);
            tmp /= 6;
        } while (tmp > 0);

        /* Interpret those digits as a base‑10 number. */
        seq_t decimal = 0;
        for (int i = (int)d - 1; i >= 0; --i)
            decimal = decimal * 10 + digits[i];

        /* If that decimal number is prime, we found a term. */
        if (is_prime_big(decimal)) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
    }
}
