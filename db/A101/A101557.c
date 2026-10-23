/*
 * A101557  Numbers k such that 101*k + 1009 is prime.
 * https://oeis.org/A101557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer k such that 101*k + 1009 is prime
   (OEIS A101557).  The sequence starts with a(1)=0. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, return placeholder */

    idx_t count = 0;                    /* how many suitable k's found */
    idx_t k = 0;                        /* candidate k */

    while (true)
    {
        /* Compute 101*k + 1009 safely in uint64_t */
        uint64_t val = 101ULL * (uint64_t)k + 1009ULL;
        if (is_prime(val))              /* is_prime is valid for 64‑bit integers */
        {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
