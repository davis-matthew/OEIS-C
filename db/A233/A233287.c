/*
 * A233287  a(n) = lcm_{i=1..n} A001177(i); partial LCM-products of Fibonacci
 *          entry points.
 * https://oeis.org/A233287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 66, u64 n <= 162,
 *            u128 n <= 352, big any n
 * Validated: standard, pass, 59 terms through n = 1016 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233287
 * Formula: a(1) = 1; a(n) = lcm( A001177(n), a(n-1) ) for n > 1
 * where A001177(n) is the smallest k such that n divides Fib(k). */

#include <stddef.h>          /* size_t */
#include <stdint.h>
#include "../utils/gcd.h"

/* Return the entry point of n in the Fibonacci sequence (A001177). */
static seq_t
entry_point(idx_t n)
{
    /* Special case: the Fibonacci sequence starts 0,1,1,2,...,
       so Fib(1)=1 and Fib(2)=1.  The entry point of 1 is 1. */
    if ((seq_t)n == (seq_t)1)
        return (seq_t)1;

    seq_t prev_mod = (seq_t)0;   /* Fib(0) mod n */
    seq_t curr_mod = (seq_t)1;   /* Fib(1) mod n */
    seq_t mod      = (seq_t)n;   /* modulus base */

    for (idx_t k = 1;; ++k)
    {
        if (curr_mod == (seq_t)0)
            return (seq_t)k;     /* n | Fib(k) */

        /* Next Fibonacci number modulo n */
        seq_t next_mod = prev_mod + curr_mod;
        if (next_mod >= mod)
            next_mod %= mod;     /* keep it small */

        prev_mod = curr_mod;
        curr_mod = next_mod;
    }
}

/* External function required by the build. */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;         /* undefined for 0, but safe */

    seq_t lcm_val = (seq_t)1;    /* a(1) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ep = entry_point(i);          /* A001177(i) */
        seq_t g  = gcd(lcm_val, ep);        /* gcd with current LCM */
        lcm_val = (lcm_val / g) * ep;       /* lcm(lcm_val, ep) */
    }

    return lcm_val;
}
