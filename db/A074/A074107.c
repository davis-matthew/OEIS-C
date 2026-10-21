/*
 * A074107  a(n) = Product of (prime + 1) for first n primes - primorial (n);
 *          Sum of proper divisors of the n-th primorial.
 * https://oeis.org/A074107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} (p_i + 1) – Π_{i=1..n} p_i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prod_plus = 1;   /* Π (prime + 1) */
    seq_t prod       = 1;   /* primorial Π prime */

    for (idx_t i = 1; i <= n; ++i)
    {
        uint32_t p  = get_prime(i);
        seq_t    ps = (seq_t)p;

        prod_plus *= (ps + 1);
        prod       *=  ps;
    }

    return prod_plus - prod;
}
