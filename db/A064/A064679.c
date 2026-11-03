/*
 * A064679  A Beatty sequence for 10*i^i.
 * https://oeis.org/A064679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( n * beta / (beta - 1) ) where beta = 10 *
 *            exp(-π/2)
 * Range:     u8 n <= 132, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 82 terms through n = 1630 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Compute the constant beta/(beta-1) once */
static long double
compute_C(void)
{
    /* high‑precision constants */
    const long double pi = acosl(-1.0L);
    const long double beta = 10.0L * expl(-pi / 2.0L);
    return beta / (beta - 1.0L);
}

/* Global static value for C to avoid recomputation */
static long double C_val = 0.0L;

/* Public sequence function */
seq_t
a(idx_t n)
{
    /* Initialise C once */
    if (C_val == 0.0L)
        C_val = compute_C();

    /* Compute n * C with long double precision */
    long double val = (long double)n * C_val;

    /* Floor and cast to integer types compatible with seq_t  */
    unsigned long long tmp = (unsigned long long)floorl(val);
    return (seq_t)tmp;
}
