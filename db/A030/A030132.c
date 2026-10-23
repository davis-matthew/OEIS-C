/*
 * A030132  Digital root of Fibonacci(n).
 * https://oeis.org/A030132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = digital_root(Fibonacci(n))
 * (The digital root equals the Fibonacci number modulo 9, with 0 mapped to 9,
 *  except for n = 0 where a(0) = 0.)
 */

#include <stdint.h>
#include "../utils/fib_mod_fast.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t fn, _;
    fib_mod_fast(n, &fn, &_);
    /* fn is Fibonacci(n) modulo 9 */
    /* Digital root: if fn == 0 -> 9, otherwise fn itself */
    return (fn == 0) ? 9 : fn;
}
