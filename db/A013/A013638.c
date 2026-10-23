/*
 * A013638  a(n) = prevprime(n)*nextprime(n).
 * https://oeis.org/A013638
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prevprime(n)*nextprime(n)
 * Range:     u8 n <= 17, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* behaviour below offset doesn't matter */

    idx_t   p = prev_prime(n);       /* previous prime ≤ n   */
    uint64_t q = next_prime(n);      /* next prime >  n     */

    return (seq_t)p * (seq_t)q;       /* product in seq_t type */
}
