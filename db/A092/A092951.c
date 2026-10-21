/*
 * A092951  Beginning with n, add the next number, subtract the previous
 *          number, and so on; then a(n) is the largest prime arising in this
 *          process, or 0 if no prime is reached in 2n-1 steps: a(n) is the
 *          largest occurring prime in the sum n + (n+1) - (n-1) + (n+2) -
 *          (n-2) + (n+3) - (n-3) + ... +- 1 occurring...
 * https://oeis.org/A092951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest prime occurring in the alternating sum n +
 *            (n+1) – (n-1) + (n+2) – (n‑2) + … − 1. If no prime appears in
 *            the 2n‑1 terms, a(n)=0
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Handle the only case where the loop below would miss the
       addition step (n = 1): a(1)=3. */
    if (n == 1)
        return (seq_t)3;

    seq_t val   = (seq_t)n;   /* current running sum */
    seq_t maxp = 0;           /* largest prime seen */

    if (isPrimeGeneric(val))
        maxp = val;

    for (idx_t k = 1; k < n; ++k) {
        /* add (n + k) */
        val += (seq_t)n + (seq_t)k;
        if (isPrimeGeneric(val) && val > maxp)
            maxp = val;

        /* subtract (n – k) */
        val -= (seq_t)n - (seq_t)k;
        if (isPrimeGeneric(val) && val > maxp)
            maxp = val;
    }
    return maxp;
}
