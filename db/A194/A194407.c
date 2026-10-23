/*
 * A194407  Numbers m such that Sum_{k=1..m} (<1/2 + k*e> - <k*e>) > 0 where <
 *          > denotes fractional part.
 * https://oeis.org/A194407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that * Sum_{k=1..m} (
 *            {1/2 + k*e} - {k*e} ) > 0. * Since {1/2 + k*e} - {k*e} equals
 *            +1/2 when {k*e}<1/2 and -1/2 otherwise, * the condition reduces
 *            to the number of k≤m with {k*e}<1/2 being strictly * more than
 *            m/2. The function computes this by iterating...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Scale for the fractional part : 10^18. */
#define MOD     1000000000000000000ULL      /* 10^18  (fits in 64‑bit) */
#define THRESH  (MOD >> 1)                  /* MOD/2 = 5×10^17 */
#define INC     718281828459045235ULL        /* floor((e-2) * MOD) */

seq_t a(idx_t n) {
    /* Counters for the routine.  All counters fit in 64‑bit unsigned. */
    idx_t m     = 0;          /* current m  (k index)      */
    idx_t A     = 0;          /* number of k with frac<1/2 */
    uint64_t frac = 0;        /* fractional part of k*e scaled by MOD */

    idx_t terms = 0;          /* how many terms found so far */

    while (1) {
        /* Advance to next m (k). */
        ++m;

        /* Update the fractional part: frac = { (k)*e } * MOD. */
        frac += INC;
        if (frac >= MOD)
            frac -= MOD;

        /* If the current fractional part is below 1/2, increment A. */
        if (frac < THRESH)
            ++A;

        /* Check the defining inequality A > m/2. */
        if (A > m / 2) {
            ++terms;
            if (terms == n)
                return (seq_t)m;
        }
    }
}
