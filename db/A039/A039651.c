/*
 * A039651  Number of iterations of f(x) = phi(x)+1 on n required to reach a
 *          prime.
 * https://oeis.org/A039651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 280, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of iterations of f(x)=phi(x)+1 until a prime is reached
   (0 if n itself is prime) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset check, not used for the series */

    seq_t x = (seq_t)n;
    seq_t steps = 0;

    while (!is_prime_big(x))
    {
        x = calc_phi(x) + 1;
        steps++;
    }
    return steps;
}
