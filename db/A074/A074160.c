/*
 * A074160  Smallest multiple of n (>n) using all prime digits; or 0 if no
 *          such number exists.
 * https://oeis.org/A074160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest multiple of n greater than n whose decimal
 *            representation consists only of prime digits 2,3,5,7; returns 0
 *            if no such multiple exists
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   all_prime_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/all_prime_digits.h"

seq_t a(idx_t n)
{
    /* If n is a multiple of 10, every multiple ends with 0 and
       cannot consist of prime digits, so the answer is 0. */
    if (n % 10 == 0)
        return (seq_t)0;

    /* Search successive multiples of n.  n ≥ 1, so the first
       candidate is 2*n.  The loop is expected to terminate quickly
       because the fraction of prime‑digit numbers that are divisible by
       n is about 1/n, and for n ≤ 10000 the answer typically has
       only a handful of digits. */
    for (idx_t k = 2;; ++k) {
        seq_t m = (seq_t)((unsigned long long)n * (unsigned long long)k);
        if (all_prime_digits(m))
            return m;
    }
}
