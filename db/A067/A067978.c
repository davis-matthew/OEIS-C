/*
 * A067978  Convolution of Fibonacci F(n+1), n>=0, with F(n+10), n>=0.
 * https://oeis.org/A067978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((199*n + 5*55)*F(n+1) + 123*(n+1)*F(n)) / 5
 * Range:     u8 none, u16 n <= 7, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* F(k) is the Fibonacci sequence: F(0)=0, F(1)=1 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* Compute F(n) and F(n+1) using fast doubling */
    seq_t f_n, f_np1;
    fib_pair(n, &f_n, &f_np1);       /* f_n = F(n), f_np1 = F(n+1) */

    /* Converting index to seq_t */
    seq_t n_seq  = (seq_t)n;
    seq_t n1_seq = n_seq + (seq_t)1;

    /* Compute the expression from the formula */
    seq_t term1 = (seq_t)199 * n_seq + (seq_t)275;          /* 199*n + 5*55 */
    seq_t term2 = (seq_t)123 * n1_seq;                     /* 123*(n+1) */

    seq_t numerator = term1 * f_np1 + term2 * f_n;
    seq_t result    = numerator / (seq_t)5;                /* exact division */

    return result;
}
