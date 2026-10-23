/*
 * A141376  Duplicate of A134517.
 * https://oeis.org/A141376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime congruent to 23 (mod 24)
 * Range:     i8 n <= 3, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t candidate = 23;
    idx_t count = 0;

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 24;
    }
}
