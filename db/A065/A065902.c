/*
 * A065902  Smallest prime p such that n is a solution mod p of x^4 = 2, or 0
 *          if no such prime exists.
 * https://oeis.org/A065902
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p > n such that n^4 ≡ 2 (mod p), or 0 if
 *            no such p exists
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 2 for the official sequence; below that the result is irrelevant */
    if (n < 2)
        return (seq_t)0;

    /* Compute m = n^4 – 2 in a type capable of holding the result.
       For n up to ≈ 10^4 this fits in 64 bits.  For larger n the
       intermediate is stored in seq_t, but we later cast to uint64_t
       because factor_u64 expects a 64‑bit argument. */
    seq_t nn = (seq_t)n;
    seq_t m = nn * nn;       /* n²  */
    m *= nn;                 /* n³  */
    m *= nn;                 /* n⁴  */
    m -= (seq_t)2;           /* n⁴ – 2 */

    /* Factor m (up to 64 bits for the intended test range). */
    uint64_t num = (uint64_t)m;
    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    factor(num, pr, exp, &cnt);   /* static helper from utils/factor.h */

    /* Scan the distinct prime factors for the smallest one exceeding n. */
    seq_t best = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        if (p > n) {
            if (best == (seq_t)0 || p < (uint64_t)best)
                best = (seq_t)p;
        }
    }

    return best;   /* 0 if none found */
}
