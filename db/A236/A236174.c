/*
 * A236174  Maximal prime among the base-k representations of the n-th prime,
 *          read in decimal, for k=2,3,...,10.
 * https://oeis.org/A236174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = maximal prime among the base-k representations of the
 *            n-th prime (k = 2,...,10)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 180, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (1-indexed). */
    uint32_t p = get_prime(n);

    /* Temporary buffer for digits of p in base k. */
    uint8_t digits[64];
    seq_t best = 0;

    /* For each base k from 2 to 10. */
    for (int k = 2; k <= 10; ++k) {
        /* Convert p to base k. */
        uint32_t tmp = p;
        int len = 0;
        do {
            digits[len++] = tmp % k;
            tmp /= k;
        } while (tmp != 0);

        /* Interpret the digits as a decimal number. */
        seq_t val = 0;
        for (int i = len - 1; i >= 0; --i) {
            val = val * 10 + digits[i];
        }

        /* If this decimal value is prime and larger than current best,
           record it. */
        if (isPrimeGeneric(val) && val > best) {
            best = val;
        }
    }

    return best;
}
