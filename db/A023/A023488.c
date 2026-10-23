/*
 * A023488  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2) and
 *          d(n) = (n-th number that is 1 or is not a Fibonacci number).
 * https://oeis.org/A023488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+4) + A001690(n)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   d_term, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000045 is the Fibonacci sequence (F0 = 0, F1 = 1) and
   A001690 enumerates 1 or non‑Fibonacci numbers:
   d(0)=1, d(1)=4, d(2)=6, d(3)=7, d(4)=9, …   */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/d_term.h"

seq_t a(idx_t n)
{
    seq_t fib_val = fib(n + 4);     /* A000045(n+4)  */
    seq_t d_val   = d_term(n);      /* A001690(n)    */
    return fib_val + d_val;
}
