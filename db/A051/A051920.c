/*
 * A051920  a(n) = binomial(n, floor(n/2)) + 1.
 * https://oeis.org/A051920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) + 1 (OEIS A051920)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Computes a(n) for any idx_t >= 0.  For builds where seq_t is 64‑bit
   or smaller, we calculate the binomial coefficient using a 128‑bit
   accumulator to avoid overflow.  For larger seq_t types (e.g. arbitrary
   precision), we delegate to the provided binomial() routine.  The
   function is safe for all n for which the result fits in seq_t. */
seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;

    /* Small seq_t: use 128‑bit math for exactness */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 res = 1;
        /* compute C(n,k) multiplicatively:
         * C(n,k) = Π_{i=1..k} (n-k+i)/i
         */
        for (idx_t i = 1; i <= k; ++i) {
            res = (res * (unsigned __int128)(n - k + i)) / i;
        }
        return (seq_t)res + (seq_t)1;
    }

    /* Otherwise (arbitrary precision or larger integral type),
       rely on the helper binomial() which returns a seq_t. */
    seq_t val = binomial(n, k);
    return val + (seq_t)1;
}
