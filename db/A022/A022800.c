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
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Return the n‑th element of the sequence of numbers that are either
   1 or not Fibonacci numbers.  The sequence begins with
   1, 4, 6, 7, 8, …  (1 is included once; Fibonacci numbers >1 are skipped). */
static seq_t nth_non_fib(idx_t n)
{
    seq_t next_i = 1;   /* current integer being examined (1 is the first term) */
    seq_t count  = 1;   /* how many qualifying numbers have been seen */
    seq_t f_prev = 1;   /* F(k‑1)  */
    seq_t f_curr = 1;   /* F(k)    */

    while (count < n) {
        /* Move to the next integer. */
        next_i++;

        /* Advance Fibonacci numbers until f_curr >= next_i. */
        while (f_curr < next_i) {
            seq_t next_f = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = next_f;
        }

        /* Count this integer if it is not a Fibonacci number greater than 1. */
        if (!(next_i > 1 && f_curr == next_i)) {
            count++;
        }
    }
    return next_i;
}

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
