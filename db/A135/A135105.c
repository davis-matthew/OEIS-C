/*
 * A135105  Digital sum (base the n-th prime) of n^5.
 * https://oeis.org/A135105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^5 - (prime(n)-1) · Σ_{k≥1} ⌊n^5 / prime(n)^k⌋
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digital sum of n^5 in base the n‑th prime.           */
/* Example: a(2) = 4, a(6) = 12                                         */

#include "../utils/get_prime.h"
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against invalid indices (optional) */
    if (n == 0) return (seq_t)0;

    /* Get the n‑th prime.  get_prime returns uint32_t */
    uint32_t p = get_prime(n);

    /* Compute n^5 using unsigned 128‑bit arithmetic to avoid overflow  */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n5 = n128 * n128 * n128 * n128 * n128;

    /* Sum floor(n^5 / p^k) for k>=1 until p^k > n^5 */
    unsigned __int128 sum_floors = 0;
    unsigned __int128 p128 = p;
    unsigned __int128 power = p128;           /* p^1          */
    while (power <= n5) {
        sum_floors += n5 / power;
        power *= p128;                       /* p^(k+1)      */
    }

    /* Compute the digital sum */
    unsigned __int128 result128 = n5 - (p128 - 1) * sum_floors;

    return (seq_t)result128;                  /* cast to seq_t */
}
