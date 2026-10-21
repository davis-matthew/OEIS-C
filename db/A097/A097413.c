/*
 * A097413  Initial decimal digit of n^9.
 * https://oeis.org/A097413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = first decimal digit of n^9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Use the fractional part of 9 · log10(n) to get the leading digit.
       Long‑double arithmetic gives enough precision for all n that are
       evaluated (e.g. n ≤ 10 000).  The result is a single digit 1…9,
       which comfortably fits in any seq_t type. */
    if (n == 0) {
        return (seq_t)0;              /* convention for n = 0, not used by OEIS */
    }

    long double logs = 9.0L * log10l((long double)n);
    long double frac = logs - floorl(logs);
    /* small epsilon guards against rounding errors that could give 9.999… */
    long double lead = powl(10.0L, frac + 1e-12L);
    int d = (int)lead;                /* d will be 1 … 9 */
    return (seq_t)d;
}
