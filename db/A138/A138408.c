/*
 * A138408  a(n) = prime(n)^6 - prime(n).
 * https://oeis.org/A138408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^6 - prime(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute the nth prime */
    uint32_t p32 = get_prime(n);   /* nth prime (1‑based) */
    seq_t p = (seq_t)p32;          /* promote to seq_t */

    /* p^6 */
    seq_t p6 = big_pow(p, (seq_t)6);

    /* a(n) = p^6 - p */
    return p6 - p;
}
