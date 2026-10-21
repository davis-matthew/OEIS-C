/*
 * A085282  Expansion of (1 - 5*x + 5*x^2)/((1-x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A085282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 1, a(1) = 3, a(2) = 10,
 *            a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3)   for n ≥ 3
 */
#include <stddef.h>   /* for size_t, used by the compiler for idx_t */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)3;
    if (n == 2ULL) return (seq_t)10;

    /* Sliding‑window recurrence using only three variables */
    seq_t t0 = (seq_t)1;   /* a(n-3) */
    seq_t t1 = (seq_t)3;   /* a(n-2) */
    seq_t t2 = (seq_t)10;  /* a(n-1) */
    seq_t t3;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 19*a(i-2) + 12*a(i-3) */
        t3 = ( (seq_t)8 * t2 )
            + ( (seq_t)12 * t0 )
            - ( (seq_t)19 * t1 );

        t0 = t1;
        t1 = t2;
        t2 = t3;
    }

    return t2;  /* a(n) */
}
