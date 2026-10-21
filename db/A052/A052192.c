/*
 * A052192  Smallest a(n) for which n*a(n) has no isolated digits.
 * https://oeis.org/A052192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_isolated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A052192 by brute‑force search:
   For each n, find the smallest a such that n*a has no isolated digits
   (in the decimal representation) that is, every digit  has a
   neighboring digit with the same value. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/no_isolated.h"

seq_t a(idx_t n)
{
    seq_t prod = 0;
    for (seq_t a_val = 1; ; ++a_val) {
        prod += (seq_t)n;                /* prod = n * a_val   */
        if (no_isolated(prod))
            return a_val;                /* desired minimal a */
    }
}
