/*
 * A214441  Catalan numbers at square positions: a(n) = A000108(n^2).
 * https://oeis.org/A214441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n^2, n^2) / (n^2 + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute k = n^2 */
    idx_t k = n * n;

    /* IF seq_t is small (≤64 bits), we try to compute exactly
       for the range that fits into 128 bits.  For larger k we
       just fall back to binomial which may overflow – that case
       is not required for the 64‑bit build. */
    if (sizeof(seq_t) <= 8) {
        if (k <= 36) {                      /* 2*k choose k fits in 128 bits */
            __uint128_t num = 1;
            for (idx_t i = 1; i <= k; ++i) {
                num = num * (k + i);
                num /= i;
            }
            num /= (k + 1);
            return (seq_t)num;
        }
        /* For k > 36 the result does not fit in 64 bits -
           we accept the overflow behaviour of the 64‑bit build. */
        seq_t res = binomial(2 * k, k);
        return res / (k + 1);
    }

    /* Arbitrary‑precision build – binomial returns the exact
       big integer value, so the division is exact. */
    seq_t res = binomial(2 * k, k);
    return res / (k + 1);
}
