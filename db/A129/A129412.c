/*
 * A129412  Numbers k such that mean of 7 consecutive squares starting with
 *          k^2 is prime.
 * https://oeis.org/A129412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k where mean of 7 consecutive squares starting with k² is
 *            prime * That mean equals (k+3)² + 4. Hence a(n) consists of all
 *            k such that (k+3)² + 4 is prime
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 4995, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t k = 0;      /* current candidate k */
    idx_t cnt = 0;    /* number of found terms */

    while (cnt < n) {
        seq_t kk = (seq_t)k;          /* cast k to seq_t for arithmetic */
        seq_t val = (kk + 3) * (kk + 3) + 4;   /* (k+3)² + 4 */

        if (isPrimeGeneric(val)) {    /* test primality */
            cnt++;
            if (cnt == n)
                return kk;           /* kth term found */
        }
        k++;                          /* try next k */
    }

    /* Should never reach here for valid n ≥ 1 */
    return (seq_t)0;
}
