/*
 * A018641  Divisors of 777.
 * https://oeis.org/A018641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th divisor of 777 in increasing order */

#include <stdbool.h>

/* Prime factors of 777 */
static const seq_t primes[3] = {3, 7, 37};

seq_t a(idx_t n)
{
    /* 777 has exactly 8 divisors.  For n outside this range return 0. */
    if (n < 1 || n > 8)
        return (seq_t)0;

    /* Generate all 8 divisors using binary masks of the 3 prime factors. */
    seq_t divs[8];
    for (idx_t mask = 0; mask < 8; ++mask) {
        seq_t prod = 1;
        if (mask & 1) prod *= primes[0];
        if (mask & 2) prod *= primes[1];
        if (mask & 4) prod *= primes[2];
        divs[mask] = prod;
    }

    /* The masks 0..7 produce the divisors in increasing order for these primes. */
    return divs[n - 1];
}
