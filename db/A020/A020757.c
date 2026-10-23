/*
 * A020757  Numbers that are not the sum of two triangular numbers.
 * https://oeis.org/A020757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   not_sum_of_two_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer m such that 4m + 1
 * contains a prime factor p ≡ 3 (mod 4) with an odd exponent.
 * This is equivalent to saying that m is NOT the sum of two triangular numbers.
 *
 * The function searches for successive integers and checks the above condition.
 * It uses trial division for factorisation, which is fast enough for the
 * required range (n up to about 10000 yields a(n) ≈ a few ×10⁴).
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/not_sum_of_two_triangular.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 1;     /* start scanning from 1 */

    while (1) {
        if (not_sum_of_two_triangular(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
