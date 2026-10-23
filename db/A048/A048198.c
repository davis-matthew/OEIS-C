/*
 * A048198  Number of primes between successive n's, where n mod 10 = 5.
 * https://oeis.org/A048198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes in the open interval * (L, U) where * L
 *            = 0, U = 5 for n = 0 * L = 5 + 10*(n-1), U = 5 + 10*n for n ≥ 1
 *            * This counts primes strictly between the successive numbers *
 *            that are ≡ 5 (mod 10)
 * Range:     u8 n <= 25, u16 n <= 6567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(seq_t) */

seq_t a(idx_t n)
{
    idx_t lower, upper;
    if (n == 0) {
        lower = 0;
        upper = 5;
    } else {
        lower = 5 + 10 * (n - 1);
        upper = 5 + 10 * n;
    }

    seq_t count = 0;
    for (idx_t p = lower + 1; p < upper; ++p) {
        if (is_prime_small((seq_t)p))
            ++count;
    }
    return count;
}
