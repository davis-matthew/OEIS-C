/*
 * A177824  a(n) = (Fibonacci(n)*Fibonacci(n+7)) mod 7.
 * https://oeis.org/A177824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fibonacci(n)*Fibonacci(n+7)) mod 7
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the fact that Fibonacci numbers modulo 7 are periodic
   with period 16: 0,1,1,2,3,5,1,6,0,6,6,5,4,2,6,1. */
#ifndef A177824_H
#define A177824_H

#include <stdint.h>   /* for uint64_t etc.  Safe to include. */

static const seq_t fib_mod7[16] = {
    (seq_t)0,(seq_t)1,(seq_t)1,(seq_t)2,(seq_t)3,(seq_t)5,
    (seq_t)1,(seq_t)6,(seq_t)0,(seq_t)6,(seq_t)6,(seq_t)5,
    (seq_t)4,(seq_t)2,(seq_t)6,(seq_t)1
};

seq_t a(idx_t n)
{
    /* Compute indices modulo the Pisano period 16 */
    seq_t fn  = fib_mod7[n & 15ULL];
    seq_t fn7 = fib_mod7[(n + 7) & 15ULL];
    /* Multiply and reduce modulo 7 */
    return (fn * fn7) % (seq_t)7;
}

#endif /* A177824_H */
