/*
 * A023496  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2) and
 *          d(n) = (n-th number that is 1 or is not a Lucas number).
 * https://oeis.org/A023496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+4) + A090946(n+1) where A000045(n) = n-th
 *            Fibonacci number (F(0)=0, F(1)=1) and A090946(n) is the n-th
 *            number that is 1 or is not a Lucas number. Here the Lucas
 *            numbers are taken as 1,3,4,7,11,18,... (i.e. the standard Lucas
 *            sequence with the initial 2 omitted)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   d_value, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/d_value.h"

/* Forward declaration of helper that returns the (n)-th element of the
   sequence of numbers that are 1 or not Lucas numbers. */
static seq_t d_value(idx_t n);

/* Compute a(n) = Fibonacci(n+4) + d(n) */
seq_t a(idx_t n)
{
    /* Fibonacci term: F(n+4) */
    seq_t fib_val = fib(n + (idx_t)4);

    /* d(n) term */
    seq_t d_val = d_value(n);

    return fib_val + d_val;
}

/* --------------------------------------------------------------------- */
