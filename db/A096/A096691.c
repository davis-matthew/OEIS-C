/*
 * A096691  Numbers n such that 8n^2 + 6n + 3 is prime.
 * https://oeis.org/A096691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth non‑negative integer m such that 8m² + 6m + 3 is
 *            prime
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 28, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The sequence starts at a(1) = 0.  We count candidates from 0 upwards. */
    idx_t target = n;
    idx_t count  = 0;
    seq_t m      = 0;

    while (true) {
        /* val = 8*m² + 6*m + 3  (computed in seq_t to avoid overflow) */
        seq_t val = (seq_t)8 * m * m + (seq_t)6 * m + (seq_t)3;
        if (is_prime_big(val)) {
            count++;
            if (count == target)
                return m;
        }
        m++;
    }
}
