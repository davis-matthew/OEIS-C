/*
 * A227957  Number of primes that are of the form n - 2^k - k^2.
 * https://oeis.org/A227957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = number of primes of the form n - 2^k - k^2 (k >= 0)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

/* Count primes of form n - 2^k - k^2 */
seq_t a(idx_t n)
{
    if (n < 2)            /* n = 1 or 0 -> no primes possible */
        return (seq_t)0;

    seq_t count = (seq_t)0;

    for (idx_t k = 0; ; ++k) {
        /* Prevent undefined behaviour from shifting by 64 or more */
        if (k >= 63)
            break;

        idx_t pow2 = 1ull << k;

        /* If 2^k alone exceeds n, further k will only increase the subtrahend */
        if (pow2 > n)
            break;

        /* Use 128‑bit intermediate to avoid overflow of k^2 + 2^k */
        unsigned __int128 sub128 = (unsigned __int128)pow2 + (unsigned __int128)k * k;

        /* Subtrahend too large for n => no point continuing */
        if (sub128 > n)
            break;

        idx_t sub = (idx_t)sub128;
        idx_t val = n - sub;          /* >= 0 due to previous check */

        /* Must be at least 2 to be a prime */
        if (val < 2)
            break;

        if (is_prime(val))
            ++count;
    }

    return count;
}
