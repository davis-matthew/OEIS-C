/*
 * A216676  Digital roots of squares of Fibonacci numbers.
 * https://oeis.org/A216676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(Fibonacci(n)^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod9_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_mod9_rec.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1 – undefined below */

    uint8_t f_mod9, dummy;
    fib_mod9_rec(n, &f_mod9, &dummy);

    uint32_t sq_mod9 = (uint32_t)f_mod9 * f_mod9 % 9u;
    return (seq_t)(sq_mod9 == 0 ? 9 : sq_mod9);
}
