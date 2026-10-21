/*
 * A090114  a(n) = Product_{j=1..n} (prime(n)-j).
 * https://oeis.org/A090114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} (prime(n)-j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* Compute the nth term of OEIS A090114 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1; return 0 as a safe default. */
        return (seq_t)0;
    }

    /* Obtain the nth prime (prime indexing starts at 1). */
    seq_t p = (seq_t)get_prime(n);

    seq_t result = (seq_t)1;
    for (idx_t j = (idx_t)1; j <= n; ++j) {
        result *= p - (seq_t)j;
    }
    return result;
}
