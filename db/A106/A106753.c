/*
 * A106753  Discriminants, negated, of definite binary quadratic forms.
 * https://oeis.org/A106753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th positive integer congruent to 0 or 3 (mod 4)
 *            that is NOT a perfect square
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t candidate = 0;

    for (;;) {
        candidate++;                           /* try next integer */
        seq_t rem = candidate & 3;              /* candidate mod 4 */
        if (rem == 0 || rem == 3) {            /* n ≡ 0 or 3 (mod 4) */
            seq_t s = isqrt(candidate);        /* floor(sqrt(candidate)) */
            if (s * s != candidate) {          /* not a perfect square */
                count++;
                if (count == (seq_t)n)         /* n-th valid number */
                    return candidate;
            }
        }
    }
}
