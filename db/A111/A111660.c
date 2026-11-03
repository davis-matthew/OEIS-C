/*
 * A111660  a(n) = greatest integer p for which n^p + (n+1)^p > (n+2)^p.
 * https://oeis.org/A111660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max p such that n^p + (n+1)^p > (n+2)^p
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Returns true if for the given n and p the inequality holds.
   Assumes n >= 2 and p >= 1. */
static inline bool holds(idx_t n, idx_t p)
{
    if (p == 0) return true;          /* 1 + 1 > 1 holds for all n */
    if (n < 2)   return false;       /* n = 0 or 1 never allow p > 0 */

    /* Work with long double for logarithms.
       Ratio (n+1)/n is <= 1.5 for n>=2, so its power stays moderate. */
    long double n_ld        = (long double)n;
    long double ratio_ld    = ((long double)(n + 1)) / n_ld;   /* <= 1.5 */
    long double ratio_pow   = powl(ratio_ld, (long double)p);

    /* left = n^p + (n+1)^p  = n^p * (1 + ratio_pow) */
    long double log_left   = (long double)p * logl(n_ld) + logl(1.0L + ratio_pow);

    /* right = (n+2)^p */
    long double log_right  = (long double)p * logl((long double)(n + 2));

    /* Strict inequality: left > right.
       Use a small epsilon to guard against rounding error. */
    return (log_left - log_right) > 1e-12L;
}

seq_t a(idx_t n)
{
    /* Handle trivial cases explicitly. */
    if (n <= 1) return (seq_t)0;

    idx_t low = 0, high = n;      /* a(n) < n for all n >= 2 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (holds(n, mid))
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
