/*
 * A089127  a(n+2) = F(n)*a(n+1) + F(n+1)*a(n) where F(n) = Fibonacci number
 *          (A000045) and a(0) = a(1) = 1.
 * https://oeis.org/A089127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n+2) = F(n) * a(n+1) + F(n+1) * a(n) where
 *            F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    */

#include <stdint.h>   /* if needed for standard integer types */

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* a_prev = a(i), a_curr = a(i+1)   initially: a_0 and a_1. */
    seq_t a_prev = 1, a_curr = 1;

    /* Fibonacci numbers for indices i and i+1.  Initially F(0)=0, F(1)=1. */
    seq_t fib_i   = 0,      /* F(i)   */
          fib_ip1 = 1;      /* F(i+1) */

    /* Iterate to build a up to index n.  We only need (n-2) iterations. */
    for (idx_t i = 0; i < n - 1; ++i) {
        /* Compute a_{i+2}. */
        seq_t a_next = fib_i * a_curr + fib_ip1 * a_prev;

        /* Shift values for next step. */
        a_prev = a_curr;
        a_curr = a_next;

        /* Advance Fibonacci numbers: F(i+1) -> F(i), F(i+2) -> F(i+1). */
        seq_t new_fib_i = fib_ip1;              /* F(i+1) becomes F(i) for next i */
        seq_t new_fib_ip1 = fib_i + fib_ip1;    /* F(i+2) = F(i) + F(i+1) */

        fib_i   = new_fib_i;
        fib_ip1 = new_fib_ip1;
    }

    return a_curr;
}
