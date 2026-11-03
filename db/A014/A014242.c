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
 * Validated: standard, pass, 49 terms through n = 969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th Fibonacci number that is not 1 (i.e. F_{n+2}) */
static seq_t fib_term(idx_t n)
{
    /* Compute F_{n+2} iteratively */
    if (n == 1) return (seq_t)2;          /* F_3 */
    if (n == 2) return (seq_t)3;          /* F_4 */

    seq_t prev = (seq_t)2;                /* F_3 */
    seq_t cur  = (seq_t)3;                /* F_4 */

    for (idx_t i = 5; i <= n + 2; ++i) {  /* up to F_{n+2} */
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}

/* Return the n-th number that is 1 or not a Fibonacci number (>1) */
static seq_t nth_non_fib_or_one(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Count how many non‑Fibonacci numbers we still need */
    idx_t remaining = n - 1;   /* 1 already accounted for */

    seq_t a = (seq_t)1;   /* F_1 */
    seq_t b = (seq_t)1;   /* F_2 */

    while (true) {
        seq_t c = a + b;           /* next Fibonacci number */
        seq_t gap = c - b - 1;     /* numbers between b+1 and c-1 (non‑Fibonacci) */

        if ((seq_t)remaining <= gap) {
            return b + (seq_t)remaining;
        }

        /* Since gap <= remaining (otherwise we would have returned),
           the conversion to idx_t is safe. */
        remaining -= (idx_t)gap;
        a = b;
        b = c;
    }
}

/* Main OEIS sequence A014242 */
extern seq_t a(idx_t n)
{
    seq_t fib_val = fib_term(n);
    seq_t nonfib_val = nth_non_fib_or_one(n);
    return fib_val - nonfib_val;
}
