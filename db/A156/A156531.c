/*
 * A156531  Denominator of Euler(n, 1/17).
 * https://oeis.org/A156531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 17^n * 2^{v2(n+1)} (where v2(x) = exponent of 2 dividing
 *            x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

static inline idx_t trailing_zeros(idx_t v) {
    /* v > 0 : count of trailing zero bits in v */
    idx_t tz = 0;
    while ((v & 1ULL) == 0ULL) {
        ++tz;
        v >>= 1;
    }
    return tz;
}

seq_t a(idx_t n) {
    /* 17^n */
    seq_t pow17 = big_pow((seq_t)17, (seq_t)n);

    /* 2^{v2(n+1)} */
    idx_t tz = trailing_zeros(n + 1);
    seq_t two_pow = ((seq_t)1) << tz;   /* tz ≤ 63 for uint64_t n */

    return pow17 * two_pow;
}
