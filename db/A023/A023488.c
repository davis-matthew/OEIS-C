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
 * Validated: standard, pass, 51 terms through n = 890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000045 is the Fibonacci sequence (F0 = 0, F1 = 1) and
   A001690 enumerates 1 or non‑Fibonacci numbers:
   d(0)=1, d(1)=4, d(2)=6, d(3)=7, d(4)=9, …   */

#include <stdbool.h>
#include <stdint.h>

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

static seq_t d_term(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* d(0) = 1 */

    /* We already know that 1 is the first element of the sequence. */
    idx_t count = 1;       /* number of terms returned so far */

    /* Fibonacci generator starting after the first 1 */
    idx_t f_prev = 1;      /* corresponds to F(1) = 1 */
    idx_t f_curr = 2;      /* corresponds to F(3) = 2 */

    for (idx_t candidate = 2; /* start from 2 */ ; ++candidate)
    {
        /* Advance the Fibonacci numbers until f_prev >= candidate */
        while (f_prev < candidate)
        {
            idx_t tmp = f_prev;
            f_prev = f_curr;
            f_curr = tmp + f_curr;
        }

        /* candidate is a Fibonacci number if it equals f_prev */
        bool is_fib = (candidate == f_prev);

        if (!is_fib)  /* candidate is not a Fibonacci number */
        {
            ++count;
            if (count == n + 1)   /* found the n‑th term (0‑based) */
                return (seq_t)candidate;
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    seq_t fib_val = fib(n + 4);     /* A000045(n+4)  */
    seq_t d_val   = d_term(n);      /* A001690(n)    */
    return fib_val + d_val;
}
