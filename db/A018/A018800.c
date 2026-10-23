/*
 * A018800  Smallest prime that begins with n.
 * https://oeis.org/A018800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime whose decimal representation begins with
 *            the digits of n
 * Range:     u8 n <= 19, u16 n <= 661, u32 n <= 620, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Return the n-th term of OEIS A018800. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0, harmless */

    seq_t digits = (seq_t)n;             /* base part (n itself)           */
    seq_t pow10  = 1;                    /* 10^k, starting with k = 0      */

    while (true) {
        seq_t start = digits * pow10;                      /* prefix * 10^k          */
        seq_t end   = start + pow10 - 1;                   /* inclusive upper limit  */
        for (seq_t cand = start; cand <= end; ++cand) {    /* scan the interval      */
            if (is_prime_big(cand))
                return cand;                              /* found smallest prime   */
        }
        pow10 *= 10;                                           /* increase k           */
    }
}
