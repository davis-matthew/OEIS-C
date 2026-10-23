/*
 * A117225  Primes of the form f(n) = 9*n^4 - 444*n^3 + 8059*n^2 - 63714*n +
 *          185371 listed by increasing value of n >= 0.
 * https://oeis.org/A117225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = f(k) where f(k) = 9*k^4 - 444*k^3 + 8059*k^2 - 63714*k + 185371
 * and k is the smallest non‑negative integer such that exactly n values of f(k)
 * have been found to be prime.  
 * Thus a(1)=f(0)=185371, a(2)=f(1)=129281, ...
 */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – behaviour for n=0 is undefined */
        return 0;

    idx_t found = 0;
    idx_t k = 0;   /* polynomial variable, starts at 0 */

    for (;;) {
        seq_t m  = (seq_t)k;
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t m4 = m3 * m;

        seq_t val =   9 * m4
                   - 444 * m3
                   + 8059 * m2
                   - 63714 * m
                   + 185371;

        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++k;
    }
}
