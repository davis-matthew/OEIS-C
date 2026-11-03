/*
 * A056585  Eighth power of Fibonacci numbers A000045.
 * https://oeis.org/A056585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^8, where F(n) is the Fibonacci number A000045
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 107 (2026-11-01)
 * Helpers:   fib_pair, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);   /* fn = F(n), fn1 = F(n+1) */
    return ipow_i32(fn, (idx_t)8);
}
