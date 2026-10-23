/*
 * A140890  a(n) = sum of primes in {10*n-9, 10*n-7, 10*n-3, 10*n-1}.
 * https://oeis.org/A140890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10n-9 isprime ? 10n-9 : 0) + (10n-7 isprime ? 10n-7 : 0)
 *            + (10n-3 isprime ? 10n-3 : 0) + (10n-1 isprime ? 10n-1 : 0)
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The four candidates for a given n */
    const idx_t offs[4] = {9, 7, 3, 1};

    /* Sum of candidates that are prime */
    seq_t sum = 0;
    idx_t base = 10 * n;

    for (int i = 0; i < 4; ++i) {
        idx_t k = base - offs[i];
        if (k >= 2 && is_prime(k))
            sum += (seq_t)k;
    }
    return sum;
}
