/*
 * A072668  Numbers one less than composite numbers.
 * https://oeis.org/A072668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers m such that m+1 is composite (i.e., m
 *            is not one less than a prime)
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple primality test for values that fit in the representation of seq_t. */
static bool is_prime(seq_t m)
{
    if (m < (seq_t)2u)
        return false;
    seq_t i = (seq_t)2u;
    for (; i <= m / i; ++i) {
        if (m % i == (seq_t)0u)
            return false;
    }
    return true;
}

/* a(n) for n >= 1. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = (seq_t)1u;
    for (;;) {
        seq_t m = x + (seq_t)1u;
        if (!is_prime(m)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
