/*
 * A160189  Prime terms subtracted from Fibonacci terms (ignoring first two
 *          terms of Fibonacci sequence).
 * https://oeis.org/A160189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fib(n + 2) - get_prime(n) (a(1) = 0)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n = 0 return 0 (unused case) */
    if (n == 0)
        return (seq_t)0;

    /* Fibonacci term at position n+2 (ignoring the first two terms) */
    seq_t f = fib(n + 2);

    /* nth prime (1‑based) */
    seq_t p = (seq_t)get_prime(n);

    return f - p;
}
