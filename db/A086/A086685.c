/*
 * A086685  Number of 1 <= i < n such that i*n+1 is prime.
 * https://oeis.org/A086685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of i (1 ≤ i < n) such that n*i+1 is prime
   (OEIS A086685). This implementation directly tests each candidate
   without any hard‑coded table. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* no i < 1 */

    idx_t count = 0;

    for (idx_t i = 1; i < n; ++i) {
        idx_t val = n * i + 1;
        if (is_prime(val))
            ++count;
    }

    return (seq_t)count;
}
