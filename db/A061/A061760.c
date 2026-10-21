/*
 * A061760  Least multiple of n that is greater than n, not equal to a power
 *          of 10 times n and contains all the digits of n.
 * https://oeis.org/A061760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 397, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies, is_power_of_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A061760:
 * the least multiple of n greater than n that is not a power of 10 times n
 * and that contains all digits of n, with the same multiplicities. */
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/is_power_of_10.h"

/* ------------------------------------------------------------------ */
/* Helper: fill an array with the number of occurrences of each digit */

/* ------------------------------------------------------------------ */
/* Helper: tests whether an integer k is a power of 10 (i.e. 10^t)    */

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t nval = (seq_t)n;
    unsigned int needed[10];
    digit_frequencies(nval, needed);   /* multiplicities of n's digits */

    for (idx_t k = 2; ; ++k) {
        if (is_power_of_10(k)) continue;            /* skip 10^t * n */
        seq_t m = nval * (seq_t)k;                  /* candidate multiple */

        /* test whether m contains all required digits with multiplicity */
        unsigned int have[10];
        for (int i = 0; i < 10; ++i) have[i] = 0;
        seq_t temp = m;
        if (temp == 0) {
            have[0] = 1;                              /* m might be 0 (cannot happen for k>=2) */
        }
        while (temp > 0) {
            unsigned int d = (unsigned int)(temp % 10);
            ++have[d];
            temp /= 10;
        }

        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (have[d] < needed[d]) { ok = false; break; }
        }
        if (ok) return m;
    }
}
