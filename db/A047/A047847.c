/*
 * A047847  Numbers n such that n + (n+1) and (n+2) + (n+3) are both prime.
 * https://oeis.org/A047847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th integer k such that 2k+1 and 2k+5 are both prime */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, but handle 0 safely */

    idx_t count = 0;
    for (idx_t k = 1; ; ++k)          /* iterate over candidate integers */
    {
        /* check the two prime conditions */
        if (is_prime(2 * k + 1) && is_prime(2 * k + 5))
        {
            ++count;
            if (count == n)
                return (seq_t)k;     /* found the required term */
        }
    }
}
