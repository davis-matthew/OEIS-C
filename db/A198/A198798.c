/*
 * A198798  Least odd number a(n) > -3 such that a(n) = p(n) - j*(j+1), where
 *          p(n) is the n-th prime.
 * https://oeis.org/A198798
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n) – j*(j+1) where p(n) is the n‑th prime and j =
 *            floor((sqrt(1 + 4*(p(n)+2)) – 1) / 2)
 * Range:     i8 n <= 663, i16 n <= 9601, i32 n <= 9601, i64 n <= 9601,
 *            i128 n <= 9601, big any n
 * Helpers:   _floor_isqrt_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Offset is 2; behaviour for n < 2 is irrelevant. */
    if (n < 2)
        return 0;

    uint32_t prime = get_prime(n);                /* nth prime, fits in 32 bits */
    idx_t val = (idx_t)1 + ((idx_t)4 * (((idx_t)prime) + 2ULL));
    idx_t s = _floor_isqrt_u64(val);
    idx_t j = (s - 1ULL) / 2ULL;

    return (seq_t)prime - (seq_t)j * ((seq_t)j + 1);
}
