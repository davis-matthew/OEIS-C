/*
 * A018551  Divisors of 615.
 * https://oeis.org/A018551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018551: a(n) = the n-th divisor of 615 in ascending order. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* There are only 8 divisors of 615: 1, 3, 5, 15, 41, 123, 205, 615. */
    /* Compute them algorithmically and sort. */
    const seq_t primes[3] = {3, 5, 41};
    const size_t np = 3;
    const size_t tot = (size_t)1 << np;        /* total divisors = 2^3 = 8 */
    seq_t divs[8];

    /* Generate all products of subsets of the prime factors. */
    for (idx_t mask = 0; mask < tot; ++mask) {
        seq_t d = 1;
        for (size_t i = 0; i < np; ++i) {
            if (mask & ((size_t)1 << i))
                d *= primes[i];
        }
        divs[mask] = d;
    }

    /* Simple insertion sort for the tiny array. */
    for (size_t i = 1; i < tot; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* Return the n-th divisor if it exists; otherwise 0. */
    if (n == 0 || n > tot)
        return (seq_t)0;
    return divs[(idx_t)(n - 1)];
}
