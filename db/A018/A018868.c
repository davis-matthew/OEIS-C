/*
 * A018868  8^a(n) is smallest power of 8 beginning with n.
 * https://oeis.org/A018868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest exponent k such that the decimal
 *            representation of 8^k starts with the integer n
 * Range:     u8 n <= 111, u16 n <= 9786, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Number of decimal digits in n.  n ≥ 1. */
static int digits_of(idx_t n)
{
    int d = 0;
    do {
        ++d;
        n /= 10;
    } while (n);
    return d;
}

/* Pre‑computed log10(8) as a long double.  All calculations below
   use long double arithmetic to keep the fractional part well
   within the limits of the format. */
static const long double LOG10_8 = log10l((long double)8.0L);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) corresponds to n = 1. */
    if (n == 1)
        return (seq_t)0;   /* 8^0 = 1 has leading digit 1 */

    /* Number of digits of the target integer */
    int d = digits_of(n);

    /* Rough lower bound for the exponent:
       k ≈ log10(n) / log10(8).  Floor gives a value that
       is guaranteed not to exceed the true answer. */
    long double approx = log10((long double)n) / LOG10_8;
    idx_t k = (idx_t)floor(approx);

    /* Linear search from the lower bound upwards until we find
       the first exponent whose leading decimal digits equal n. */
    for (;;) {
        /* Compute k·log10(8) as a long double */
        long double lg = (long double)k * LOG10_8;
        /* Number of decimal digits of 8^k */
        int digits_8k = (int)floorl(lg) + 1;

        /* If 8^k still has fewer digits than n, skip it. */
        if (digits_8k < d) {
            ++k;
            continue;
        }

        /* Fractional part of lg */
        long double frac = lg - floorl(lg);

        /* Leading d digits of 8^k:
           10^(frac + d – 1)  →  10^(frac) * 10^(d‑1)  */
        long double tmp = powl(10.0L, frac + (long double)(d - 1));
        int lead = (int)(tmp + 1e-12L);   /* small epsilon to guard
                                            against rounding errors */

        if (lead == n)
            return (seq_t)k;

        /* Always increment k; the sequence is increasing in n,
           so the correct exponent will be reached eventually. */
        ++k;
    }
}
