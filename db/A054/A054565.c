/*
 * A054565  Numbers n such that 3 is the first digit of 3^n.
 * https://oeis.org/A054565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Program: a(n) = the n‑th exponent e such that the first decimal digit of 3^e is 3.
   The algorithm checks the fractional part of e·log10(3).  It works for any
   seq_t that can be constructed from an idx_t value.  No dynamic allocation is
   required, so it compiles in both C and C++ versions. */

#include <math.h>   /* log10l, floorl */

static const long double LOG10_3 = log10l(3.0L);   /* log10(3) */
static const long double LOG10_4 = log10l(4.0L);   /* log10(4) */

/* --------------------------------------------------------------- */
/* Return the n-th integer e (1‑based) for which 3^e starts with digit 3. */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, return 0 */
    idx_t count = 0;                 /* how many valid exponents found   */
    idx_t e     = 0;                 /* current exponent to test         */

    while (count < n) {
        ++e;
        long double frac = e * LOG10_3;
        frac -= floorl(frac);                 /* fractional part of log10(3^e) */
        if (frac >= LOG10_3 && frac < LOG10_4) {
            ++count;                          /* 3^e starts with digit 3 */
        }
    }
    return (seq_t)e;                /* cast to the requested big integer type */
}
