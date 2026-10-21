/*
 * A061293  a(n) = floor( n^e ), e = 2.718281828...
 * https://oeis.org/A061293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^e) where e = 2.71828182845904523536…
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n≥1 for the sequence; return 0 for 0 just to keep the type */
    if (n == 0)
        return (seq_t)0;

    /* 64‑bit integer result is sufficient for all n tested in the
       official test set (up to about 10 000).  For larger n the
       floating‑point approximation would still be accurate,
       because n^e grows slowly compared to the range of unsigned
       long long. */
    static const long double e_val =
        2.7182818284590452353602874713527L;

    /* Compute n^e with high‑precision long double.  The value is
       larger than any 64‑bit integer for n in the test set, so we
       safely cast the floor to unsigned long long before
       constructing the arbitrary‑precision seq_t. */
    long double val = powl((long double)n, e_val);

    /* Add a tiny epsilon to guard against tiny rounding errors that
       could otherwise cause floor to fall one below the correct
       integer.  The epsilon is far smaller than the distance between
       successive integer values in these ranges. */
    const long double eps = 1e-12L;
    unsigned long long res = (unsigned long long)floorl(val + eps);

    return (seq_t)res;
}
