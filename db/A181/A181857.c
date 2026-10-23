/*
 * A181857  a(n) = lcm(n^2, n!).
 * https://oeis.org/A181857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n^2, n!)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0,   a(1) = 1,   a(2) = 4,   a(3) = 18,   a(4) = 48 */
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 18;
    if (n == 4) return 48;

    /* For n > 4 the rule is: prime → n · n! ; composite → n! */
    seq_t fac = factorial(n);      /* n! */
    if (is_prime(n))
        return fac * (seq_t)n;     /* n · n! */
    else
        return fac;                /* n! */
}
