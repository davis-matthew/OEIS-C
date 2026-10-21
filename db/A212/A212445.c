/*
 * A212445  a(n) = floor( n + log(n) ).
 * https://oeis.org/A212445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n + log(n) )
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1. */

#include <math.h>

seq_t a(idx_t n)
{
    /* Convert the index to double for the logarithm */
    double val = (double)n + log((double)n);

    /* Floor the result; a tiny epsilon guards against round‑off errors */
    unsigned long long tmp = (unsigned long long)(val + 1e-12);

    /* Cast the small integer to the required seq_t type */
    return (seq_t)tmp;
}
