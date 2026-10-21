/*
 * A050183  T(2n+5,n), array T as in A051168; a count of Lyndon words.
 * https://oeis.org/A050183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(2*n+5, n) / (2*n+5) if n%5 != 0 (binom(2*n+5, n) -
 *            binom(2*(n/5)+1, n/5)) / (2*n+5) if n%5 == 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    idx_t denom = 2*n + 5;          /* 2*n+5 fits in idx_t for all n in range */
    seq_t res = binom(denom, n);    /* binomial(2*n+5, n) */

    if ((n % 5) == 0) {             /* subtract the second binomial when 5|n */
        idx_t k = n / 5;
        seq_t sub = binom(2*k + 1, k);
        res = res - sub;
    }

    return res / (seq_t)denom;      /* exact division, result is an integer */
}
