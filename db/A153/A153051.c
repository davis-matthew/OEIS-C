/*
 * A153051  Numbers n>=9 such that 2*n-17 is not a prime.
 * https://oeis.org/A153051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   not_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153051: numbers n ≥ 9 such that 2·n − 17 is not prime. */
/* The function returns a(n) for n ≥ 1 (offset 1). */

#include <stdbool.h>
#include "../utils/not_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t cand = 9; ; ++cand) {
        seq_t val = (seq_t)2 * cand - (seq_t)17;
        if (not_prime(val)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
    }
}
