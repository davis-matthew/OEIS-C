/*
 * A068695  Smallest number (not beginning with 0) that yields a prime when
 *          placed on the right of n.
 * https://oeis.org/A068695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A068695
 *  a(n) = smallest integer m (not starting with 0) such that
 *  concatenating n and m yields a prime.
 *  The search iterates over odd m, updating 10^k as the digit length
 *  of m increases, and uses isPrimeGeneric() to test primality.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /*  n is guaranteed >=1 by the sequence definition.  */
    seq_t nseq = (seq_t)n;        /* convert index to seq_t */
    seq_t m      = 1;              /* candidate suffix (odd) */
    seq_t pow10  = 10;             /* 10^digits(m) for current m */

    while (1) {
        /* When m grows to a new digit length, increase pow10.
         * (m employs only odd values; 10 itself is skipped.) */
        if (m >= pow10) {
            pow10 *= 10;
        }

        /* Form the concatenated number n * 10^k + m. */
        seq_t cand = nseq * pow10 + m;

        /* If the concatenation is prime, we have found the minimal m. */
        if (isPrimeGeneric(cand)) {
            return m;
        }

        /* Try the next odd number. */
        m += 2;
    }
}
