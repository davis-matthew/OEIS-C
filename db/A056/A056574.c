/*
 * A056574  Seventh power of Fibonacci numbers A000045.
 * https://oeis.org/A056574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^7, where F(n) is the n‑th Fibonacci number (F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    /* fib_pair sets fn = F(n) and fn1 = F(n+1) */
    fib_pair(n, &fn, &fn1);

    seq_t res = 1;
    for (int i = 0; i < 7; ++i)
        res *= fn;   /* compute F(n)^7 */

    return res;
}
