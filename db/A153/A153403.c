/*
 * A153403  Numbers k such that 10*k+3 is not prime.
 * https://oeis.org/A153403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k where k is the nth non‑negative integer for which
 *            10*k+3 is not prime
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        seq_t val = (seq_t)10 * (seq_t)k + 3;
        if (!is_prime_big(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* unreachable, but avoid compiler warnings */
    return (seq_t)0;
}
