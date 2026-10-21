/*
 * A067445  Smallest n-th power starting with 4.
 * https://oeis.org/A067445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = smallest integer k such that k^n starts with digit 4;
   returns the power k^n. */
#include <math.h>
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */

static const double L4 = log10(4.0);          /* log10(4)  */
static const double L5 = log10(5.0);          /* log10(5)  */
static const double EPS = 1e-12;

seq_t a(idx_t n)
{
    /* Search for the minimal base k whose n-th power begins with 4. */
    for (idx_t k = 1; ; ++k) {
        double f = n * log10((double)k);
        f -= floor(f);                          /* fractional part of n·log10(k) */

        /* 4 ≤ 10^f < 5  →  log10(4) ≤ f < log10(5)  */
        if (f >= L4 - EPS && f < L5 - EPS) {
            /* Returned value fits within seq_t by construction. */
            return seq_pow((seq_t)k, n);
        }
    }
}
