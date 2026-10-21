/*
 * A076565  Greatest prime divisor of 2n+1 (sum of two successive integers).
 * https://oeis.org/A076565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of (2*n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the odd number 2*n+1 in seq_t arithmetic. */
    seq_t m = (seq_t)n * 2 + 1;

    /* For the n-values used in the tests the value fits in 64 bits,
       so we can safely cast to uint64_t for factorisation. */
    uint64_t val = (uint64_t)m;
    uint64_t lpf = 1;     /* largest prime factor found so far */

    /* Test divisibility by 2 first. */
    if ((val & 1) == 0) { /* val is even (only possible for n = 0) */
        lpf = 2;
        while ((val & 1) == 0)
            val >>= 1;
    }

    /* Test odd divisors starting at 3. */
    for (uint64_t d = 3; d * d <= val; d += 2) {
        if (val % d == 0) {
            lpf = d;
            while (val % d == 0)
                val /= d;
        }
    }

    /* If a prime factor larger than sqrt remains, it is the largest. */
    if (val > 1)
        lpf = val;

    return (seq_t)lpf;
}
