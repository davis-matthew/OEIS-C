/*
 * A125603  Numbers n such that 3n(n-1)/2 + 1 is prime.
 * https://oeis.org/A125603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125603:  numbers n such that 3*n*(n-1)/2 + 1 is prime.
   a(1) = 4, a(2) = 5, a(3) = 9, …  (see OEIS for first terms) */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined offset – graceful fallback */
        return 0;

    idx_t count = 0;    /* how many valid numbers we found so far */

    for (idx_t m = 1; ; ++m) {
        /* compute p = 3*m*(m-1)/2 + 1 in a type that can hold the value */
        unsigned long long val = 3ULL * m * (m - 1ULL) / 2ULL + 1ULL;

        /* test primality of p – cast to seq_t so the helper can work
           with the type provided by the build system */
        if (is_prime_big((seq_t)val)) {
            ++count;
            if (count == n)
                return (seq_t)m;          /* found the n-th term */
        }
    }
}
