/*
 * A100780  Primes with decimal representation beginning 2357...
 * https://oeis.org/A100780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime whose decimal representation begins with
 *            the prefix 2357 (the prime 2357 itself is not counted). The
 *            function searches numbers in increasing order, testing each for
 *            primality until the n-th such prime is found
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    const uint64_t PREFIX = 2357ULL;
    idx_t count = 0;

    uint64_t pow10 = 1ULL;          /* 10^(len-4) */
    uint64_t base   = PREFIX;       /* PREFIX * pow10 */

    /* iterate over lengths len = 4, 5, 6, ... */
    for (;;)
    {
        for (uint64_t suffix = 0; suffix < pow10; ++suffix)
        {
            uint64_t cand = base + suffix;

            /* skip the 4‑digit prime 2357 itself */
            if (cand != PREFIX && is_prime_u64(cand))
            {
                ++count;
                if (count == n)
                    return (seq_t)cand;
            }
        }

        /* move to the next length */
        pow10 *= 10ULL;
        base   = PREFIX * pow10;
    }
}
