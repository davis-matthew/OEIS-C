/*
 * A120225  a(n) is the minimal number k>n such that n+k and n*k+1 are primes.
 * https://oeis.org/A120225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k > n such that n+k and n*k+1 are primes
 * Range:     u8 n <= 12, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast once for speed       */
    idx_t k  = n + 1;               /* start searching > n       */

    while (1) {
        seq_t sum = nn + (seq_t)k;          /* n + k                  */
        if (!is_prime((idx_t)sum)) {       /* if n+k not prime       */
            ++k;
            continue;
        }

        seq_t prod = nn * (seq_t)k + 1;     /* n*k + 1                */
        if (is_prime((idx_t)prod)) {        /* if both primes found   */
            return (seq_t)k;
        }

        ++k;
    }
}
