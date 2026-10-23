/*
 * A014242  (n-th Fibonacci number that is not 1) - (n-th number that is 1 or
 *          not a Fibonacci number).
 * https://oeis.org/A014242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-th Fibonacci number that is not 1) - (n-th number that
 *            is 1 or not a Fibonacci number)
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   fib_term, nth_non_fib_or_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_term.h"
#include "../utils/nth_non_fib_or_one.h"

/* Main OEIS sequence A014242 */
extern seq_t a(idx_t n)
{
    seq_t fib_val = fib_term(n);
    seq_t nonfib_val = nth_non_fib_or_one(n);
    return fib_val - nonfib_val;
}
