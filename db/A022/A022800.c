/*
 * A022800  a(n) = F(n+2) + c(n) where F(k) is k-th Fibonacci number and c(n)
 *          is n-th number that is 1 or is a non-Fibonacci number.
 * https://oeis.org/A022800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+2) + c(n), where F(k) is the k‑th Fibonacci number
 *            (F(0)=0,F(1)=1) and c(n) is the n‑th integer that is 1 or not a
 *            Fibonacci number. The sequence starts with a(1)=3, a(2)=7, … as
 *            listed in OEIS A022800
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   fib, nth_non_fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/nth_non_fib.h"

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Fibonacci part: F(n+2).  The universal fib() routine works for
       any build: arbitrary‑precision on 128‑bit builds, and standard
       unsigned types on 8/16/32/64‑bit builds. */
    seq_t f = fib(n + 2);

    /* Non‑Fibonacci part: c(n). */
    seq_t c = nth_non_fib(n);

    return f + c;
}
