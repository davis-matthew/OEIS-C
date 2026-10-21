/*
 * A062082  a(n) = prime(n)^n * n^prime(n).
 * https://oeis.org/A062082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n * n^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1, but guard against zero */
        return 0U;

    /* obtain the nth prime (1‑indexed) */
    uint32_t p = get_prime(n);          /* p fits in 32‑bit */

    /* compute p^n and n^p */
    seq_t pow1 = pow_uint((uint64_t)p, n);          /* prime^n */
    seq_t pow2 = pow_uint((uint64_t)n, (idx_t)p);   /* n^prime */

    /* multiply the two parts */
    return pow1 * pow2;
}
