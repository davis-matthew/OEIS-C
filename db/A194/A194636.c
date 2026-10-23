/*
 * A194636  Least k >= 0 such that (2*n-1)*2^k - 1 or (2*n-1)*2^k + 1 is
 *          prime, or -1 if no such value exists.
 * https://oeis.org/A194636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k >= 0 such that (2*n-1)*2^k - 1 or (2*n-1)*2^k + 1
 *            is prime; returns -1 if no such k exists
 * Range:     i8 n <= 21, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return -1;                /* sequence defined only for n >= 1 */

    seq_t base = (seq_t)n * 2 - 1;/* 2*n-1 */
    seq_t pow2 = 1;
    const int MAX_K = 2000;      /* safety cap to avoid infinite loops */

    for (int k = 0; k <= MAX_K; ++k) {
        seq_t M = base * pow2;   /* current (2*n-1)*2^k */

        if (is_prime_big(M - 1) || is_prime_big(M + 1))
            return k;           /* found the least such k */

        pow2 = pow2 * 2;        /* next power of two */
    }

    return -1;                  /* no k found within the search limit */
}
