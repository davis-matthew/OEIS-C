/*
 * A102343  Numbers k such that k*10^3 + 777 is prime.
 * https://oeis.org/A102343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer k such that 1000*k + 777 is prime
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t count = 0;              /* how many valid k found so far */
    idx_t k = 1;                  /* candidate k */

    while (count < n) {
        /* k that is a multiple of 3 can be skipped because 1000*k + 777
           would then be divisible by 3. */
        if (k % 3 != 0) {
            seq_t cand = (seq_t)k * (seq_t)1000 + (seq_t)777;
            if (is_prime_seq(cand)) {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)k;
}
