/*
 * A208742  Number of subsets of the set {1,2,...,n} which do not contain two
 *          elements whose difference is 5.
 * https://oeis.org/A208742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(floor(n/5)+3)^(n mod 5) * F(floor(n/5)+2)^(5 - (n mod
 *            5))
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the Fibonacci routine from the utils package. */

#include "../utils/fib.h"

/* Small exponentiation with exponent up to 5. */
static inline seq_t pow_small(seq_t base, int exp) {
    seq_t result = (seq_t)1;
    for (int i = 0; i < exp; ++i) {
        result = result * base;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t q = n / 5;
    int r = (int)(n % 5);

    seq_t high = fib(q + 3);
    seq_t low  = fib(q + 2);

    seq_t p_high = pow_small(high, r);          /* F(q+3)^r          */
    seq_t p_low  = pow_small(low, 5 - r);       /* F(q+2)^(5-r)      */

    return p_high * p_low;                      /* product gives a(n) */
}
