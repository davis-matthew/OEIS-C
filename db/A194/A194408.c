/*
 * A194408  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=Pi and < > denotes fractional part.
 * https://oeis.org/A194408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Sequence: a(n) = the nth integer m such that
 * 2 * #{k ≤ m : fractional part of k*π < 0.5} < m.
 * The sum Σ_{k=1}^m (⟦1/2 + kπ⟧ – ⟦kπ⟧) is negative iff the inequality holds.
 */
static const long double PI = 3.141592653589793238462643383279502884L;
static const long double EPS = 1e-12L;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used; offset is 1 */
    idx_t found = 0;
    idx_t m     = 0;
    idx_t less  = 0;                  /* count of k with {kπ} < 0.5 */

    while (found < n) {
        ++m;
        /* compute fractional part of m*π */
        long double val = (long double)m * PI;
        long long  ipart = (long long)(val + EPS);    /* floor with safety */
        long double frac = val - (long double)ipart;   /* 0 ≤ frac < 1 */

        if (frac < 0.5L) ++less;

        /* 2*less < m  <=>  S(m) < 0 */
        if ((unsigned __int128)2 * less < m) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
    }
    return 0;   /* never reached */
}
