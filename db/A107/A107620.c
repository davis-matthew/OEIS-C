/*
 * A107620  Primes multiplied alternately by 3 and 2.
 * https://oeis.org/A107620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)*(2 + mod(n, 2))
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; if called with 0 return 0 */
    if (n == 0)
        return 0;
    seq_t p = get_prime(n);            /* p = nth prime, 1‑based */
    seq_t factor = 2 + (seq_t)(n & 1); /* 3 when n odd, 2 when n even */
    return p * factor;                 /* exact multiplication in seq_t */
}
