/*
 * A071424  a(n) = a(n-1) + number of decimal digits of n!. Number of decimal
 *          digits of concatenation of first n factorials.
 * https://oeis.org/A071424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 1, a(n) = a(n‑1) + number_of_digits(n!) (n ≥ 2)
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: compute the number of decimal digits of a factorial by accumulating logs */
static int digit_count_of_factorial(idx_t n, long double *logsum)
{
    /* log10(n!) = Σ_{k=1}^n log10(k)  */
    /* floor(log10(n!)) + 1 gives the digit count.  Use a small epsilon to avoid
       floor inaccuracies at integer boundaries. */
    if (n == 0) return 0;          /* not used for the sequence but keeps the function generic */
    long double value = logsum[n];
    return (int)floor(value + 1e-12L) + 1;
}

seq_t a(idx_t n)
{
    static seq_t *pref = NULL;      /* pref[i] holds a(i) */
    static long double *logsum = NULL; /* logsum[i] = log10(i!) */
    static idx_t max_n = 0;         /* largest n for which pref/logsum are valid */

    if (n == 0) return 0;           /* offset is 1; guard against illegal 0 index */

    /* Allocate (or resize) storage if needed */
    if (n > max_n) {
        size_t new_len = (size_t)(n + 1); /* indices 0 … n */
        pref    = mem_resize(seq_t, pref, new_len);
        logsum  = (long double *)realloc(logsum, new_len * sizeof(long double));

        /* First call: initialise everything */
        if (max_n == 0) {
            pref[0]    = 0;
            logsum[0]  = 0.0L;
            /* factorial(1) = 1 */
            logsum[1]  = 0.0L;                         /* log10(1) = 0 */
            pref[1]    = 1;                            /* digits(1!) = 1 */
            max_n = 1;
        }

        /* Compute new values up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            logsum[i] = logsum[i - 1] + log10l((long double)i);
            int digits = digit_count_of_factorial(i, logsum);
            pref[i]    = pref[i - 1] + (seq_t)digits;
        }
        max_n = n;
    }

    return pref[n];
}
