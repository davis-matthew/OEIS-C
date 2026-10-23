/*
 * A179546  a(n) = p^2*(p + 3)/2, where p = prime(n).
 * https://oeis.org/A179546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 * (prime(n) + 3) / 2
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 308, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 1.  For n = 0 the behaviour
       is undefined, so we simply return 0 in that case. */
    if (n == 0)
        return 0;

    /* Retrieve the n‑th prime (1‑indexed). */
    uint32_t p = get_prime(n);

    seq_t np = (seq_t)p;
    seq_t result = np * np;          /* p^2 */
    result = result * (np + 3) / 2;  /* * (p+3) / 2 */

    return result;
}
