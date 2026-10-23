/*
 * A193841  Erroneous version of A227590.
 * https://oeis.org/A193841
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = F_{n+2} - 1 for 2 ≤ n ≤ 5, a(n) = F_n + 2 for n > 5 (F_k
 *            denotes the k‑th Fibonacci number)
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 5)
        return fib(n + 2) - 1;
    else
        return fib(n) + 2;
}
