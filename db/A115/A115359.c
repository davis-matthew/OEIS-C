/*
 * A115359  Matrix (1,x)-(x,x^2) in Riordan array notation.
 * https://oeis.org/A115359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = A010054(n) - A379480(n) Equivalent: a(n) = 1 if n =
 *            k(k+3)/2 (k ≥ 0) a(n) = -1 if n = 2k^2 - 1 (k ≥ 1) otherwise 0
 * Range:     i8 n <= 9824, i16 n <= 9824, i32 n <= 9824, i64 n <= 9824,
 *            i128 n <= 9824, big any n
 * Helpers:   is_perfect_square128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_perfect_square128.h"

seq_t a(idx_t n)
{
    /* Check the +1 case: n = k(k+3)/2  →  k^2 + 3k - 2n = 0
       ⇒ discriminant D = 9 + 8n must be a perfect square. */
    __uint128_t D = (__uint128_t)9 + (__uint128_t)8 * n;
    uint64_t rootD;
    if (is_perfect_square128(D, &rootD)) {
        if (rootD >= 3 && ((rootD - 3) & 1ULL) == 0) {
            /* k = (rootD - 3)/2 ≥ 0 */
            return (seq_t)1;
        }
    }

    /* Check the -1 case: n = 2k^2 - 1  →  k^2 = (n+1)/2
       Only possible if n is odd. */
    if ((n & 1ULL) == 1) {                     /* n is odd → n+1 even */
        __uint128_t m = (__uint128_t)n + 1;     /* (n+1)/2 in 128‑bit */
        m >>= 1;
        uint64_t rootM;
        if (is_perfect_square128(m, &rootM)) {
            if (rootM >= 1)                     /* k≥1 */
                return (seq_t)-1;
        }
    }

    return (seq_t)0;
}
