/*
 * A145279  Fecundity of n-th Fibonacci number.
 * https://oeis.org/A145279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = number of iterations required to reach a zero‑digit
 *            number * by repeatedly adding the product of the decimal digits
 *            of the n‑th Fibonacci * number F(n). a(0) = 0. For n ≥ 184 the
 *            value is 0
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   fib_u128, has_zero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib_u128.h"
#include "../utils/has_zero_digits.h"

seq_t a(idx_t n)
{
    /* Fecundity is zero for all n >= 184 */
    if (n >= 184) return 0;

    unsigned __int128 cur = fib_u128(n);
    seq_t steps = 0;

    while (!has_zero_digits(cur)) {
        /* product of decimal digits */
        unsigned __int128 temp = cur;
        unsigned __int128 prod = 1;
        while (temp > 0) {
            prod *= (unsigned int)(temp % 10);
            temp /= 10;
        }
        cur += prod;
        ++steps;
    }

    return (seq_t)steps;
}
