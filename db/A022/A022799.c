/*
 * A022799  a(n) = F(n+1) + c(n) where F(k) is k-th Fibonacci number and c(n)
 *          is n-th non-Fibonacci number.
 * https://oeis.org/A022799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+1) + c(n) where c(n) is the n‑th non‑Fibonacci number
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci part */
    seq_t fib_part = fib(n + 1);

    /* Find the n-th non‑Fibonacci number c(n) */
    seq_t n_seq      = (seq_t)n;         /* the target index, cast to seq_t  */
    seq_t count_non  = 0;                /* cumulative count of non‑Fibonacci numbers so far */
    seq_t f_prev = 1;                    /* F(k-1)   */
    seq_t f_curr = 1;                    /* F(k)     */

    while (1) {
        seq_t f_next   = f_prev + f_curr;   /* F(k+1)   */
        seq_t interval = f_next - f_curr - 1; /* numbers between F(k) and F(k+1) */

        if (count_non + interval >= n_seq) {
            seq_t offset  = n_seq - count_non;   /* 1‑based offset inside the interval */
            seq_t c_n     = f_curr + offset;     /* the n‑th non‑Fibonacci number   */
            return fib_part + c_n;               /* a(n) = F(n+1) + c(n)            */
        }
        count_non += interval;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
