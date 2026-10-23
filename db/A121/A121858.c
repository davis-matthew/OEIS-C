/*
 * A121858  Smallest odd number having prime(n) divisors, where prime(n) is
 *          the n-th prime=A000040(n).
 * https://oeis.org/A121858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(prime(n)-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1: undefined for n < 1 */
    uint32_t p = get_prime(n);    /* nth prime */
    return pow3((idx_t)p - 1);    /* 3^(p-1) */
}
