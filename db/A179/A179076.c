/*
 * A179076  Number of primes of the form k^2 + 1 less than n.
 * https://oeis.org/A179076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of primes of the form k^2+1 that are strictly less than n.
   a(n) = |{ k >= 1 : k^2+1 < n and k^2+1 is prime }|    */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n < 2 → no primes less than n */
    if (n <= 2) return 0;

    seq_t count = 0;
    for (idx_t k = 1;; ++k)
    {
        /* k^2+1 can overflow only for astronomical n; here n is tiny */
        idx_t val = k * k + 1;
        if (val >= n) break;          /* strictly less than n */
        if (is_prime(val))            /* primality test */
            ++count;
    }
    return count;
}
