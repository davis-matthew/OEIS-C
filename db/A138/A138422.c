/*
 * A138422  a(n) = (prime(n)^4 - prime(n)^2)/12.
 * https://oeis.org/A138422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^4 - prime(n)^2)/12
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 91, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses 4 consecutive factors of prime(n) to avoid overflows
   and performed all divisions by 2 and 3 before multiplication. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-based index). */
    seq_t p = (seq_t)get_prime(n);

    /* Build the four factors: p-1, p, p+1, p. */
    seq_t f[4] = { p - 1, p, p + 1, p };

    /* Divide by 2 twice (12 = 2 * 2 * 3). */
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ((f[j] & 1) == 0) {          /* even */
                f[j] >>= 1;
                break;
            }
        }
    }

    /* Divide by 3 once. */
    for (int j = 0; j < 4; ++j) {
        if (f[j] % 3 == 0) {
            f[j] /= 3;
            break;
        }
    }

    /* Multiply the reduced factors. */
    seq_t res = 1;
    for (int j = 0; j < 4; ++j) {
        res *= f[j];
    }

    return res;
}
