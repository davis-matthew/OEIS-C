/*
 * A074495  a(n) = the first prime > sigma(n).
 * https://oeis.org/A074495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the first prime > σ(n)
 * Range:     u8 n <= 95, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(n) is the sum of the positive divisors of n */

/* Required utilities */
#include <stdbool.h>
#include "../utils/sigma_of.h"      /* seq_t sigma_of(idx_t n) */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n) */

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* search for the least prime strictly greater than σ(n) */
    seq_t p = sigma + 1;
    while (!isPrimeGeneric(p))
        ++p;

    return p;
}
