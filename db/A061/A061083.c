/*
 * A061083  Fibonacci-type sequence based on division: a(0) = 1, a(1) = 2 and
 *          a(n) = a(n-2)/a(n-1) but ignore decimal point.
 * https://oeis.org/A061083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = k^(Fib(n)) where k = 2 if n is odd, k = 5 if n is even.
 *            Fib(n) is the n‑th Fibonacci number with Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;            /* fn = Fib(n), fn1 = Fib(n+1) – unused. */
    fib_pair(n, &fn, &fn1);   /* Compute Fib(n).                      */
    seq_t base = (n & 1ULL) ? (seq_t)2 : (seq_t)5;
    return big_pow(base, fn);
}
