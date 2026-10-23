/*
 * A196815  Smallest index k such that prime(k+n) - prime(k) is a perfect
 *          square.
 * https://oeis.org/A196815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A196815:
 * a(n) = smallest index k such that prime(k+n) - prime(k) is a perfect square.*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

/* Check if a seq_t value is a perfect square. */
static bool is_perfect_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    for (idx_t k = 1; ; ++k)
    {
        /* Get the k-th and (k+n)-th primes. */
        uint32_t pk   = get_prime(k);
        uint32_t pk_n = get_prime(k + n);

        seq_t diff = (seq_t)pk_n - (seq_t)pk;
        if (is_perfect_square(diff))
            return (seq_t)k;
    }
}
