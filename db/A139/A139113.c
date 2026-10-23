/*
 * A139113  Concatenation of n and n-th Fibonacci number.
 * https://oeis.org/A139113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(n, Fib(n)) (decimal concatenation of n and the
 *            n-th Fibonacci number)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   digit_len, fib, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Return the n-th term of OEIS sequence A139113: concatenation of n and Fib(n) */
seq_t a(idx_t n)
{
    /* Compute the n-th Fibonacci number */
    seq_t fn = fib(n);

    /* Number of decimal digits in fn */
    int d = digit_len(fn);

    /* 10^d  (big integer) */
    seq_t pow10_d = pow10(d);

    /* Concatenate: n * 10^d + fn */
    return (seq_t)n * pow10_d + fn;
}
