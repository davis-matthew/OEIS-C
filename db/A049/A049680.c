/*
 * A049680  a(n) = (L(n) + L(2*n))/2, where L = A000032 (the Lucas sequence).
 * https://oeis.org/A049680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A049680
 *  a(n) = (L(n) + L(2*n))/2
 *  where L is the Lucas sequence: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2).
 *  The function uses the provided Fibonacci helper fib() to compute Lucas
 *  numbers via the identity L(n) = F(n-1) + F(n+1).
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Compute the n-th Lucas number using Fibonacci. */
static seq_t lucas_from_fib(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)2;
    if (n == (idx_t)1) return (seq_t)1;
    /* L(n) = F(n-1) + F(n+1) */
    seq_t fn_minus1 = fib(n - (idx_t)1);
    seq_t fn_plus1  = fib(n + (idx_t)1);
    return fn_minus1 + fn_plus1;
}

/* Compute a(n) for OEIS sequence A049680. */
seq_t a(idx_t n)
{
    idx_t two_n   = n << 1;            /* 2*n */
    seq_t ln       = lucas_from_fib(n);
    seq_t l2n      = lucas_from_fib(two_n);
    seq_t sum      = ln + l2n;         /* sum may overflow seq_t,
                                         but it is reduced by /2 afterwards */
    return sum / (seq_t)2;
}
