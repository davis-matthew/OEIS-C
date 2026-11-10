/*
 * A220519  Permutation of prime numbers in the order of sequential reading
 *          the antidiagonals of A220508.
 * https://oeis.org/A220519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime in the sequence A220508 (square array T(i,j) =
 *            i^2+j if j<=i, else j*(j+2)-i, read by ascending antidiagonals);
 *            computed by scanning antidiagonals and testing primality
 * Range:     u8 n <= 31, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t d = 0;; d++) {
        for (uint64_t t = 0; t <= d; t++) {
            uint64_t i = d - t;
            uint64_t j = t;
            uint64_t v;
            if (j <= i) v = i * i + j;
            else        v = j * (j + 2) - i;
            if (v >= 2 && is_prime_u64(v)) {
                count++;
                if (count == n) return (seq_t)v;
            }
        }
    }
}
