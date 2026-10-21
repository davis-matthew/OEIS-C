/*
 * A092847  a(n) = Sum_{k=1..n} round(prime(k) / log(prime(k))).
 * https://oeis.org/A092847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} round(prime(k) / log(prime(k)))
 * Range:     u8 n <= 24, u16 n <= 389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the series from scratch for each call. */

#include <math.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* n=0 is outside the official offset */

    seq_t sum = 0;          /* cumulative sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* prime(k) is provided by the helper routine */
        uint64_t p = get_prime(k);

        /* compute p / log(p) as a double, round it, and add to sum */
        double val = (double)p / log((double)p);
        double r = floor(val + 0.5);          /* nearest integer */

        /* cast the rounded double back to uint64_t then to seq_t */
        sum = sum + (seq_t)((uint64_t)r);
    }

    return sum;
}
