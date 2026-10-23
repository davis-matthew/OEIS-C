/*
 * A025719  Index of 6^n within the sequence of the numbers of the form
 *          6^i*10^j.
 * https://oeis.org/A025719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log10(6))
 * Range:     u8 n <= 24, u16 n <= 409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for idx_t, seq_t types */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* high‑precision constant for log10(6) */
    const double LOG10_6 = 0.778151250383644629726321655;

    const double EPS = 1e-12;   /* tolerance for rounding */
    seq_t result = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        double x = (double)k * LOG10_6;          /* k * log10(6)  */
        unsigned long long v = (unsigned long long)x; /* floor(x)      */
        if ((double)v + EPS < x) v++;            /* ceil(x)       */
        result += (seq_t)v;                      /* accumulate    */
    }
    return result;
}
