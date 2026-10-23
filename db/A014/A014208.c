/*
 * A014208  Next prime after n-th Fibonacci number.
 * https://oeis.org/A014208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   fib, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the next prime after the n-th Fibonacci number */

#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n);               /* nth Fibonacci number */
    seq_t cand = f + 1;             /* start search after fib(n) */

    while (!isPrimeGeneric(cand))
        cand = cand + 1;            /* increment until prime */

    return cand;
}
