/*
 * A214787  a(n) is the smallest k>=2 such that n divides Fibonacci(k-1)+5, or
 *          zero if no such k exists.
 * https://oeis.org/A214787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A214787: a(n) is the smallest k ≥ 2 such that n | Fibonacci(k‑1)+5,
   or 0 if no such k exists.  Uses Pisano period (≤ 6n). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of domain; return 0 */

    /* Work modulo n; cast n to the arbitrary‑precision seq_t type. */
    seq_t modn = (seq_t)n;

    /* Fibonacci initial values: F(0)=0, F(1)=1. */
    seq_t f_prev = (seq_t)0;       /* F(k-2) */
    seq_t f_curr = (seq_t)1;       /* F(k-1) */

    /* k = 2 corresponds to F(1).  Test it first. */
    if (((f_curr + (seq_t)5) % modn) == 0) return 2;

    /* Upper bound for Pisano period is 6n.  That guarantees a full
       cycle for any modulus n. */
    idx_t limit = 6 * n;
    for (idx_t k = 3; k <= limit; ++k) {
        /* Compute next Fibonacci number modulo n. */
        seq_t f_next = (f_prev + f_curr) % modn;
        f_prev = f_curr;
        f_curr = f_next;

        if (((f_curr + (seq_t)5) % modn) == 0) return k;
    }
    return 0;   /* no k found within the Pisano period */
}
