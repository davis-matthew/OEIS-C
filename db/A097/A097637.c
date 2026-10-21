/*
 * A097637  a(n) = Fibonacci(floor(n/16) + BinaryWeight(n mod 16)).
 * https://oeis.org/A097637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, dumb
 * Formula:   a(n) = Fib( ⌊n/16⌋ + popcount(n mod 16) ), Fib(0)=0, Fib(1)=1
 *            (popcount = number of 1‑bits of the lower 4 bits of n)
 * Range:     u8 n <= 174, u16 n <= 350, u32 n <= 718, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute Fibonacci number with F(0)=0, F(1)=1 */
static seq_t fib(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i) {
        seq_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
    }
    return f1;
}

seq_t a(idx_t n)
{
    /* floor(n/16) */
    idx_t q = n >> 4;
    /* popcount of (n mod 16) */
    unsigned m = (unsigned)(n & 0xF);      /* 4 lower bits of n */
    unsigned w = 0;
    while (m) { w++; m &= m - 1; }          /* Brian Kernighan's algorithm */
    /* Fibonacci index */
    idx_t fidx = q + w;
    return fib(fidx);
}
