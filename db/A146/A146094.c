/*
 * A146094  Bell numbers (A000110) read mod 4.
 * https://oeis.org/A146094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Bell(n) mod 4.
 * The sequence is periodic with period 12: a(n) = a(n-12) for all n ≥ 12.
 */

#include <stdint.h>  /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* precompute the first 12 Bell numbers modulo 4 */
    static seq_t bell[12];
    static int initialized = 0;

    if (!initialized) {
        bell[0] = 1; /* Bell(0) */
        for (idx_t i = 1; i < 12; ++i) {
            unsigned long long sum = 0;
            unsigned long long C = 1;           /* (i-1 choose 0) */
            unsigned long long N = i - 1;       /* n-1 in the recurrence */
            for (idx_t k = 0; k <= i - 1; ++k) {
                /* add term: Bell(k) * C(n-1, k) */
                sum += (unsigned long long)bell[k] * C;
                /* update the binomial coefficient for the next k */
                if (k < i - 1) {
                    /* C = C * (N - k) / (k + 1) */
                    C = C * (N - k) / (k + 1);
                }
            }
            bell[i] = (seq_t)(sum % 4);
        }
        initialized = 1;
    }

    /* use the periodicity: a(n) = a(n % 12) */
    idx_t r = n % 12;
    return bell[r];
}
