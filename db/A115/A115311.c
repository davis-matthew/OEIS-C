/*
 * A115311  a(n) = gcd(Lucas(n)-1, Fibonacci(n)-1).
 * https://oeis.org/A115311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(Lucas(n) - 1, Fibonacci(n) - 1)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 46, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   fib_pair, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Corresponds to OEIS offset: a(1) is the first term (0). */
    if (n <= 1)
        return 0;  /* a(1) = 0, and for n==0 we also return 0 by safety */

    seq_t Fn, Fn1;               /* Fn  = F_n, Fn1 = F_{n+1} */
    fib_pair(n, &Fn, &Fn1);

    /* Lucas(n) = 2*F_{n+1} - F_n  */
    seq_t lucas = (seq_t)2 * Fn1 - Fn;

    /* Compute gcd of (Lucas(n)-1) and (Fibonacci(n)-1) */
    return gcd(lucas - 1, Fn - 1);
}
