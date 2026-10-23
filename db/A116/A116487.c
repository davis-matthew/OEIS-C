/*
 * A116487  First digit after decimal point in decimal representation of
 *          (1+1/n)^n.
 * https://oeis.org/A116487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10*(1+1/n)^n) mod 10
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first decimal digit of (1+1/n)^n via long double arithmetic. */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Compute (1 + 1/n)^n accurately in long double (approx. 80‑bit). */
    long double base = 1.0L + 1.0L / (long double)n;
    long double pow_val = expl(n * logl(base));

    /* Scale by 10 and take the integer part. */
    long double scaled = pow_val * 10.0L;
    unsigned long long q = (unsigned long long)floorl(scaled);

    /* Return the last decimal digit. */
    return (seq_t)(q % 10ULL);
}
