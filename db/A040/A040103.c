/*
 * A040103  Duplicate of A045323.
 * https://oeis.org/A040103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime p such that p % 8 != 5
 * Range:     i8 n <= 22, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t candidate = 2;
    idx_t idx = 0;

    while (true) {
        if (is_prime(candidate)) {
            if ((candidate & 7u) != 5u) {  /* p % 8 != 5 */
                if (idx == n) {
                    return (seq_t)candidate;
                }
                ++idx;
            }
        }
        ++candidate;
    }
}
