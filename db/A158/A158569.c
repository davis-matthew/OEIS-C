/*
 * A158569  a(n) = Sum_{i=1..F(n)} F(i), where F = A000045, Fibonacci numbers.
 * https://oeis.org/A158569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   fib_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS:   A158569
   Formula: a(n) = Sum_{i=1..F(n)} F(i) = F(F(n)+2) - 1
   where F = A000045 (F(0)=0, F(1)=1, ...).
   This implementation computes F(n) for an idx_t index and F(k) for an
   arbitrary-size seq_t index using fast doubling.  No external lookup
   tables are used. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/fib_big.h"

/* Compute Fibonacci number F(n) with a 64‑bit integer index.
   Uses fast doubling and only seq_t arithmetic. */
static seq_t fib_small(idx_t n)
{
    seq_t a = (seq_t)0;   /* F(0)   */
    seq_t b = (seq_t)1;   /* F(1)   */

    /* Find highest power of two <= n */
    idx_t power = (idx_t)1;
    while (power << 1 <= n)
        power <<= 1;

    /* Process bits from MSB to LSB */
    for (idx_t mask = power; mask > 0; mask >>= 1)
    {
        /* Doubling formulas:
           c = F(2k)   = a*(2*b - a)
           d = F(2k+1) = a*a + b*b   */
        seq_t two_b = b + b;
        seq_t two_b_minus_a = two_b - a;
        seq_t c = a * two_b_minus_a;
        seq_t d = a * a + b * b;

        if (n & mask) {          /* bit is 1: k -> 2k+1 */
            a = d;
            b = c + d;
        } else {                 /* bit is 0: k -> 2k   */
            a = c;
            b = d;
        }
    }
    return a;   /* F(n) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t fn = fib_small(n);           /* F(n) as seq_t */
    seq_t m  = fn + (seq_t)2;          /* index for outer Fibonacci */
    seq_t fm = fib_big(m);             /* F(F(n)+2) */
    return fm - (seq_t)1;              /* a(n) = F(F(n)+2) - 1 */
}
