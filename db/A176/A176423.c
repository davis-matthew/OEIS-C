/*
 * A176423  Numbers that are the sum of at least three distinct nonnegative
 *          integers in arithmetic progression.
 * https://oeis.org/A176423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A176423.
 *
 * a(n) = 3, if n = 1
 * a(n) = 6, if n = 2
 * otherwise a(n) is the (n−2)-th composite integer greater than 8.
 *
 * The implementation performs a simple linear search, testing each
 * integer for primality with the helper function is_prime(idx_t).
 * This is efficient enough for the required range (n up to about 10 000)
 * and works for all larger n as well. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    idx_t target = n - 2;          /* zero‑based index among composites > 8 */
    idx_t candidate = 9;
    idx_t count = 0;

    for (;; ++candidate) {
        if (!is_prime(candidate)) {
            if (count == target - 1) return (seq_t)candidate;
            ++count;
        }
    }
}
