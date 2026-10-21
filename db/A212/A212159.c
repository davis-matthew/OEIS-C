/*
 * A212159  a(n) = (-1)^((prime(n) + 1)/2).
 * https://oeis.org/A212159
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^((prime(n)+1)/2)
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* seq_t and idx_t are defined by the build system.
   The function returns +1 if the nth prime is 3 (mod 4),
   otherwise –1 (for primes 1 (mod 4) when n ≥ 2). */
seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    return (p & 3U) == 3U ? (seq_t)1 : (seq_t)-1;
}
