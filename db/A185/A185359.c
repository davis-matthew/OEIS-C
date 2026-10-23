/*
 * A185359  Numbers k such that {m^m mod k: m >= 1} is not purely periodic.
 * https://oeis.org/A185359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 8903, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_large_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185359:
   a(n) is the n-th positive integer k that contains a prime power pᵉ
   with exponent e > p.  The sequence is increasing. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_large_exponent.h"

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t k = 1; ; ++k) {
        if (has_large_exponent(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
