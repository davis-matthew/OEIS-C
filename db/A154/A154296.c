/*
 * A154296  Primes of the form (1+2+3+...+m)/15 = A000217(m)/15, for some m.
 * https://oeis.org/A154296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154296: primes of the form (1+2+...+m)/15 = m*(m+1)/30
 * Enumerated in increasing order of m.  The sequence is finite:
 * the only possible m values that yield an integer are m≡0,2,5,7,14,16,19,21 (mod 30),
 * and only m= 9,14,29,30 give primes.  This routine searches for them
 * algorithmically, so no hard‑coded table is used. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    idx_t count = 0;
    /* A very generous upper bound; the sequence has only 4 terms. */
    for (idx_t m = 1; m <= 100000; ++m) {
        seq_t prod = (seq_t)m * (seq_t)(m + 1);
        if (prod % 30 != 0) continue;        /* need integer result */

        seq_t val = prod / 30;
        if (is_prime_big(val)) {
            ++count;
            if (count == n) return val;
        }
    }
    /* No further terms exist; return 0 for n > #terms. */
    return 0;
}
