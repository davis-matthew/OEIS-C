/*
 * A035514  Zeckendorf expansion of n: repeatedly subtract the largest
 *          Fibonacci number you can until nothing remains. Big-endian
 *          concatenation of decimals.
 * https://oeis.org/A035514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of decimal forms of the Zeckendorf
 *            expansion of n using Fibonacci numbers 1, 2, 3, 5, 8, …
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 372, u64 n <= 4179,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Remaining amount to represent */
    seq_t remaining = (seq_t)n;

    /* Fibonacci numbers up to n (max 93 for 64-bit n) */
    seq_t fibs[128];
    int fib_cnt = 0;
    seq_t f1 = 1, f2 = 2;

    fibs[fib_cnt++] = f1;
    if (remaining >= 2) {
        fibs[fib_cnt++] = f2;
    }

    while (remaining >= f2) {
        seq_t f3 = f1 + f2;
        if (f3 > remaining) break;
        fibs[fib_cnt++] = f3;
        f1 = f2;
        f2 = f3;
    }

    /* Construct the concatenated number in descending Fibonacci order */
    seq_t result = 0;
    for (int i = fib_cnt - 1; i >= 0; --i) {
        if (fibs[i] <= remaining) {
            seq_t mult = pow10((idx_t)digit_len(fibs[i]));
            result = result * mult + fibs[i];
            remaining -= fibs[i];
        }
    }

    return result;
}
