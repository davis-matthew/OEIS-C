/*
 * A038399  Concatenate first n nonzero Fibonacci numbers in reverse order.
 * https://oeis.org/A038399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = concatenation of the first n non‑zero Fibonacci numbers
 *            in reverse order, i.e. a(n) = F(n) || F(n-1) || … || F(1) (here
 *            "||" denotes decimal concatenation)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   digit_len, fib, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* External linkage: required by the build system. */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0. */
    if (n < 1) return 0;

    /* Base case n = 1. */
    seq_t result = fib(1);
    if (n == 1) return result;

    /* Build the concatenation iteratively (prepending each Fibonacci
       number).  At step i we prepend F(i) to the current result.
       Prepending is implemented as F(i) * 10^{digits(result)} + result. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Number of decimal digits of the current result. */
        idx_t d = digit_len(result);
        seq_t shift = pow10(d);        /* 10^d */

        /* Prepend the next Fibonacci number. */
        result = fib(i) * shift + result;
    }

    return result;
}
