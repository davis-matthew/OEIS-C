/*
 * A106785  Primes with digit sum = 58.
 * https://oeis.org/A106785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime whose decimal digit sum equals 58
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_sum_uint64, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime64.h"
#include "../utils/digit_sum_uint64.h"

seq_t a(idx_t n)
{
    const uint64_t TARGET_DIGITSUM = 58;
    if (n == 0) return 0;               /* undefined for n<1, return 0 */
    uint64_t count = 0;                   /* number of qualifying primes found */
    uint64_t candidate = 3;               /* start from the first odd prime */

    while (1) {
        /* skip numbers ending in 5 (except 5 itself), they cannot be prime >5 */
        if (candidate % 10 == 5) {
            candidate += 2;
            continue;
        }

        if (digit_sum_uint64(candidate) == TARGET_DIGITSUM &&
            isPrime64(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }

        candidate += 2;   /* only odd numbers need be tested */
    }
}
