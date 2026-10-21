/*
 * A232009  a(n) = the smallest squarefree number (from A005117) of the form
 *          p*q with prime factors in a p^2 + n progression, or 0 if no such
 *          number exists.
 * https://oeis.org/A232009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest squarefree number m = p*q * with p prime, q
 *            = p² + n prime, * or 0 if no such m exists
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Odd n: only p = 2 can give odd q = p² + n. */
    if ((n & 1) != 0) {
        idx_t q = n + 4;           /* 2² + n */
        if (is_prime(q))
            return (seq_t)2 * (seq_t)q;
        else
            return 0;
    }

    /* Even n: p must be an odd prime (p = 2 gives even q > 2). */
    const idx_t MAX_P = 2000000;    /* safety bound for prime search */
    for (idx_t p = 3; p <= MAX_P; p += 2) {
        if (!is_prime(p))
            continue;

        idx_t q = p * p + n;       /* p² + n */
        if (is_prime(q))
            return (seq_t)p * (seq_t)q;
    }
    return 0;                       /* no suitable p found within the bound */
}
