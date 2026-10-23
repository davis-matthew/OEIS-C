/*
 * A008465  2^(2n-6) * C(n,3) - 2^(n-2) * C(n,4).
 * https://oeis.org/A008465
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n-6) * C(n,3) - 2^(n-2) * C(n,4) where C(n,k) is the
 *            binomial coefficient. The offset of the sequence is 3 (a(3) is
 *            the first term)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute n choose 3 in a big‑integer/get‑unsigned way. */
static seq_t binom3(idx_t n) {
    /* n*(n-1)*(n-2)/6  – exact for all n>=3. */
    seq_t res = (seq_t)n * (n-1) * (n-2) / 6;
    return res;
}

/* Compute n choose 4 in a big‑integer/get‑unsigned way. */
static seq_t binom4(idx_t n) {
    /* n*(n-1)*(n-2)*(n-3)/24  – exact for all n>=4. */
    seq_t res = (seq_t)n * (n-1) * (n-2) * (n-3) / 24;
    return res;
}

/* Compute 2^e by repeated left shifts.  Works for any seq_t
   implementation that supports the << operator. */
static seq_t pow2_u(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) {
        r <<= 1;
    }
    return r;
}

/* Main sequence function.  It can be called in any order and
   in any number of times; no memoisation is needed. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  For smaller n we
       return 0, though those values are outside the OEIS offset. */
    if (n < 3) {
        return 0;
    }

    seq_t term1 = pow2_u(2 * n - 6) * binom3(n);
    seq_t term2 = pow2_u(n - 2) * binom4(n);
    return term1 - term2;
}
