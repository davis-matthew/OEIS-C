/*
 * A060418  Largest decimal digit in n-th prime.
 * https://oeis.org/A060418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = largest decimal digit of the n-th prime (A000040)
 * Range:     u8 n <= 9951, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   get_prime, largest_digit_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* Provides: uint32_t get_prime(idx_t k); */
#include "../utils/largest_digit_uint32.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety, return 0 for n < 1. */
    if (n < 1) return 0;
    uint32_t p = get_prime(n);
    return largest_digit_uint32(p);
}
