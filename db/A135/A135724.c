/*
 * A135724  Fibonacci numbers whose indices are prime Fibonacci numbers: a(n)
 *          = Fibonacci(Fibonacci(A001605(n))).
 * https://oeis.org/A135724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(Fibonacci(A001605(n)))
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   fib, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A135724.                      */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed. */
    idx_t count = 0;
    seq_t prev2 = 1; /* F(1) */
    seq_t prev1 = 1; /* F(2) */
    seq_t result = 0;

    for (idx_t i = 3; ; ++i) {
        seq_t f = prev2 + prev1;  /* F(i) = F(i-1) + F(i-2) */
        prev2 = prev1;
        prev1 = f;

        if (is_prime_big(f)) {
            /* f fits into idx_t because it is a known prime Fibonacci number
               whose value is <= 2^64-1 for all entries that the sequence
               contains. */
            result = fib((idx_t)f);
            ++count;
            if (count == n)
                break;
        }
    }

    return result;
}
