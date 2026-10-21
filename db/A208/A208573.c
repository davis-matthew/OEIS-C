/*
 * A208573  Smallest twin prime > 3*2^n.
 * https://oeis.org/A208573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 14, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest twin prime > 3*2^n */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the threshold 3*2^n */
    seq_t threshold = (seq_t)3 * pow2(n);

    /* Start searching with the first odd integer > threshold */
    seq_t cand = threshold + 1;
    if ((cand & 1) == 0) cand += 1;

    /* Scan odd numbers until we find a twin prime pair (cand, cand+2) */
    while (true) {
        if (is_prime_big(cand) && is_prime_big(cand + 2))
            return cand;
        cand += 2;
    }
}
