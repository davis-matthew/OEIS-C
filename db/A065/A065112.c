/*
 * A065112  Smallest prime whose decimal expansion ends (nontrivially) with
 *          the n-th prime; or 0 if no such prime exists.
 * https://oeis.org/A065112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime > prime(n) whose decimal expansion ends
 *            with prime(n); returns 0 if no such prime exists (this occurs
 *            when prime(n) ends with 2 or 5)
 * Range:     u8 n <= 6, u16 n <= 168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     // undefined, but safe fallback

    /* obtain the n‑th prime (fits in 32 bits for n ≤ 10000) */
    uint32_t p = get_prime(n);

    /* if the prime ends with 2 or 5, no larger prime can end with it */
    uint32_t last = p % 10;
    if (last == 2 || last == 5)
        return 0;

    /* determine number of decimal digits of p */
    uint32_t tmp = p;
    uint32_t d = 0;
    do { d++; tmp /= 10; } while (tmp);

    /* compute 10^d as seq_t */
    seq_t pow10 = 1;
    for (uint32_t i = 0; i < d; ++i)
        pow10 *= 10;

    /* search for the first prime > p ending with p */
    seq_t candidate = p + pow10;      /* k = 1 */
    for (;;)
    {
        if (isPrime((idx_t)candidate))
            return candidate;
        candidate += pow10;           /* next candidate with same suffix */
    }
}
