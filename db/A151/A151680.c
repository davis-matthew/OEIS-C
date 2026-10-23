/*
 * A151680  a(n) = sum_{k >= 0} binomial(wt(n+k),k+1), where wt() = A000120().
 * https://oeis.org/A151680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k >= 0} binomial( A000120(n + k), k + 1 )
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) as defined in OEIS A151680. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; ; ++k) {
        idx_t tn = n + k;
        idx_t wt = __builtin_popcountll(tn);

        /* Stop when k + 1 exceeds the popcount, since binomial would be zero. */
        if ((k + 1) > wt)
            break;

        sum += binomial(wt, k + 1);
    }

    return sum;
}
