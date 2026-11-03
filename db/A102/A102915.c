/*
 * A102915  Numbers n such that n3 is prime and n is a multiple of 10.
 * https://oeis.org/A102915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th multiple of 10 such that (n*10+3) is a prime
   (a(1)=0).  This implements the OEIS A102915 sequence. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of found terms so far */
    idx_t m     = 0;          /* current multiple of 10 (candidate n) */

    while (count < n) {
        /* Form the number to test: 10*m + 3 */
        uint64_t cand = ((uint64_t)m * 10ULL) + 3ULL;
        if (is_prime_u64(cand)) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m += 10;              /* next multiple of 10 */
    }

    /* This point is not reached for n >= 1 */
    return (seq_t)0;
}
