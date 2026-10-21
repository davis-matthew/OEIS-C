/*
 * A055034  a(1) = 1, a(n) = phi(2*n)/2 for n > 1.
 * https://oeis.org/A055034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055034
 *
 *  Formula: a(1) = 1; for n > 1,
 *  a(n) = φ(2n) / 2
 */

/* No external headers are needed beyond the standard ones used by the
 * build environment.  All operations are done with seq_t, which on a
 * 64‑bit build is at least uint64_t but may also be an arbitrary‑precision
 * integer type in a C++17 build. */

#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Utility: compute Euler's totient of a seq_t value.  This works for
 * all supported seq_t types because it only uses +, -, *, /, % and
 * comparisons.                                                        */
static seq_t
phi_of(seq_t m)
{
    seq_t result = m;
    seq_t temp   = m;

    /* start with the smallest odd factor (2), then test successive
     * integers as potential prime factors.  The condition
     *  temp / p >= p  replaces the usual p*p <= temp check and avoids
     *  overflow. */
    seq_t p = (seq_t)2;
    while ((temp / p) >= p) {
        if (temp % p == 0) {
            /* Remove all powers of p from temp. */
            while (temp % p == 0) {
                temp /= p;
            }
            /* Update result: result = result / p * (p-1). */
            result = result / p;
            result = result * (p - (seq_t)1);
        }
        ++p;
    }
    /* If something is left in temp, it must be a prime > sqrt(m). */
    if (temp > (seq_t)1) {
        result = result / temp;
        result = result * (temp - (seq_t)1);
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Sequence function required by the build system.  n is an unsigned
 * 64‑bit integer (idx_t).  The function returns a seq_t value. */
seq_t
a(idx_t n)
{
    if (n <= (idx_t)1) {
        return (seq_t)1;          /* a(1) = 1 */
    }

    /* Compute φ(2n) and divide by two. */
    seq_t m          = (seq_t)n;            /* cast n to seq_t   */
    m               *= (seq_t)2;            /* m = 2*n           */
    seq_t phi_value  = phi_of(m);           /* phi(2n)           */
    return phi_value / (seq_t)2;            /* integer division  */
}
