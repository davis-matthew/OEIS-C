/*
 * A161522  prime(n)*( prime(n)-n ).
 * https://oeis.org/A161522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*(prime(n)-n)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* n < 1 is outside the sequence; return 0 as guard */
    uint32_t p = get_prime(n);        /* nth prime */
    uint64_t diff = (uint64_t)p - (uint64_t)n;  /* prime(n) - n */
    uint64_t prod = (uint64_t)p * diff;         /* product */
    return (seq_t)prod;
}
