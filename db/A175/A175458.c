/*
 * A175458  a(0)=0. a(n) = a(n-1) - n, if a(n-1) - n = a (positive) prime.
 *          Otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A175458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, a(n) = a(n-1) - n if a(n-1)-n is a positive
 *            prime, otherwise a(n) = a(n-1) + n
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t cur = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* only subtract when cur >= i, so the result is non‑negative */
        if (cur >= (seq_t)i) {
            seq_t cand = cur - (seq_t)i;
            if (cand > 0 && is_prime_big(cand))
                cur = cand;          /* decrease */
            else
                cur = cur + (seq_t)i;/* increase */
        } else {
            cur = cur + (seq_t)i;    /* increase */
        }
    }
    return cur;
}
