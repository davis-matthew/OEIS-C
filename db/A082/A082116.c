/*
 * A082116  Fibonacci sequence (mod 5).
 * https://oeis.org/A082116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 5, with a(0)=0, a(1)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 20: 0,1,1,2,3,0,3,3,1,4,0,4,4,3,2,0,2,2,4,1 */

#include <stddef.h>

static const seq_t fib_mod5_period[20] = {
    0, 1, 1, 2, 3,
    0, 3, 3, 1, 4,
    0, 4, 4, 3, 2,
    0, 2, 2, 4, 1
};

seq_t a(idx_t n)
{
    /* Reduce n modulo the Pisano period 20 and return the pre‑computed value. */
    return fib_mod5_period[(size_t)(n % 20)];
}
