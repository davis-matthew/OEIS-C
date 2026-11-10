/*
 * A206640  Primes in A060432.
 * https://oeis.org/A206640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   search -- a(n) = n-th prime among A060432(m) = (k-1)k(2k-1)/6 +
 *            j*k, 1 <= j <= k, k >= 1 (finite list)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t T = 0;            /* sum of i^2 for i < k */
    seq_t k = 1;
    for (;;)
    {
        seq_t v = T;
        for (seq_t j = 1; j <= k; j++)
        {
            v += k;
            if (is_prime_seq(v))
            {
                count++;
                if (count == n)
                    return v;
            }
        }
        T = v;
        k++;
    }
}
