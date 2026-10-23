/*
 * A116770  Number of permutations of length n which avoid the patterns 1243,
 *          1342, 4312.
 * https://oeis.org/A116770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(-99/16 + 29*n/32 + 3*n^2/32) + n^2 + 4*n + 6, n > 1
 *            with a(1) = 1. Uses the closed‑form expression
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure access to the 2^k routine */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* offset 1 */
    if (n == 1) return (seq_t)1;

    /* small n handled with 64‑bit arithmetic to avoid negative
       intermediate values in unsigned seq_t */
    if (n < 5) {
        int64_t nn = (int64_t)n;
        int64_t val = (int64_t)1 << n;                 /* 2^n, fits in 64 bit when n<5 */
        int64_t num = -198 + 29*nn + 3*nn*nn;          /* -198 + 29n + 3n^2 */
        int64_t term = val * num / 32;                 /* term1 */
        int64_t res  = term + nn*nn + 4*nn + 6;         /* final value */
        return (seq_t)res;
    }

    /* n >= 5 : use big‑integer arithmetic with only positive operations*/
    seq_t n2 = (seq_t)n * (seq_t)n;                     /* n^2 */
    seq_t part = (seq_t)3 * n2 + (seq_t)29 * (seq_t)n;  /* 3n^2 + 29n */
    seq_t s = part - (seq_t)198;                        /* 3n^2 + 29n - 198 (non‑negative) */

    /* 2^n / 32 = 2^(n-5) */
    seq_t pow_shift = pow2(n - 5);                       /* 2^(n-5) */

    seq_t term1 = pow_shift * s;                        /* 2^(n-5) * (3n^2 + 29n - 198) */
    seq_t res = term1 + n2 + (seq_t)4 * (seq_t)n + (seq_t)6;

    return res;
}
