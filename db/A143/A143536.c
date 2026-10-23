/*
 * A143536  Triangle read by rows, T(n,k) = 1 if n is prime, 0 otherwise.
 * https://oeis.org/A143536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the row index m, defined by m(m+1)/2 ≥ n, is prime;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 8001, u16 n <= 8001, u32 n <= 8001, u64 n <= 8001,
 *            u128 n <= 8001, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the smallest m such that m*(m+1)/2 >= n. */
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until the condition holds. */
    while (( (__int128)high * (high + 1) ) / 2 < n) {
        high <<= 1;
    }

    /* Binary search for the exact m. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sum = (__int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }

    /* low now holds the correct row index m */
    return (seq_t)(is_prime(low) ? 1 : 0);
}
