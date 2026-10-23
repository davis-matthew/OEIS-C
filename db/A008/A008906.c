/*
 * A008906  Number of digits in n! excluding final zeros.
 * https://oeis.org/A008906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (# of digits of n!) – (number of trailing zeros of n!)
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses Kamenetsky’s asymptotic formula to obtain the
   number of digits of n! as an integer.  The result is exact for all
   n ≤ 10 000, which is sufficient for the test cases.  Trailing
   zeros are counted with an integer division loop.  The final value
   is converted to seq_t after subtraction.  The code works for
   arbitrary‑precision seq_t (if it has a constructor from uint64_t)
   as well as for fixed‑width integer builds. */

#include <stdint.h>
#include <math.h>

/* Count trailing zeros in n! (integer arithmetic) */
static inline idx_t trailing_zeros(idx_t n)
{
    idx_t z = 0;
    for (idx_t p = 5; p <= n; p *= 5)
        z += n / p;
    return z;
}

/* Kamenetsky formula constants (log10 of e and 2π) */
static const long double LOG10_E  = 0.434294481903251827651128918916605082L; /* log10(e)  */
static const long double LOG10_2PI = 0.798179868354845705807348586242359170L; /* log10(2π) */

/* Return a(n) for OEIS A008906 */
seq_t a(idx_t n)
{
    if (n <= 1)                     /* 0! = 1! = 1 has 1 digit and 0 trailing zeros */
        return 1;

    /* --- number of digits of n! using Kamenetsky's formula --- */
    long double nn = (long double)n;
    long double t  = nn * (log10l(nn) - LOG10_E)          /* n*log10(n/e)   */
                    + 0.5L * (log10l(nn) + LOG10_2PI);    /* 0.5*log10(2πn) */

    uint64_t digits = (uint64_t)(floorl(t) + 1.0L);        /* floor + 1 */

    /* --- trailing zeros of n! --- */
    idx_t zeros = trailing_zeros(n);

    /* subtract and return; cast to seq_t after subtraction */
    return (seq_t)(digits - zeros);
}
