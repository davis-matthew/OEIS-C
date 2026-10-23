/*
 * A039650  Prime reached by iterating f(x) = phi(x)+1 on n.
 * https://oeis.org/A039650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A039650: iterate f(x)=phi(x)+1 until a prime is found.
   a(n) is the limit of this sequence; the function always terminates
   because phi(x) < x for composite x. */
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    while (!is_prime_big(x))
        x = 1 + phi((idx_t)x);      /* apply f(x)=phi(x)+1 */
    return x;
}
