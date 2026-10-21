/*
 * A213198  Number of iterations of the map n -> f(f(f(...f(n)...))) to reach
 *          the end of the cycle, where f(n) = A006577(n), the initial number
 *          n is not counted.
 * https://oeis.org/A213198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes a(n) as defined in OEIS A213198:  a(n) is the
 * number of times the map f is applied until the result is 1 or 5,
 * where f(n) = A006577(n) (the total Collatz stopping time for n). */
#include <stdint.h>
#include <stdbool.h>

/* Compute the total Collatz stopping time for a given n. */
static seq_t collatz_steps(idx_t start)
{
    seq_t steps = 0;
    seq_t x = (seq_t)start;
    while (x != (seq_t)1) {
        if ((x & (seq_t)1) == (seq_t)0) {      /* even: divide by 2 */
            x /= (seq_t)2;
        } else {                               /* odd: 3*x + 1 */
            x = x * (seq_t)3 + (seq_t)1;
        }
        ++steps;
    }
    return steps;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1 || n == (idx_t)5) {
        return (seq_t)0;
    }

    idx_t current = n;
    seq_t count = 0;

    while (current != (idx_t)1 && current != (idx_t)5) {
        seq_t steps = collatz_steps(current);   /* f(current) */
        current = (idx_t)steps;                 /* cast is safe: steps < 2^64 */
        ++count;
    }

    return count;
}
