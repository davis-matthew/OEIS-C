/*
 * A218251  G.f. satisfies A(x) = (1 + x*A(x))^2 * (1 + x^3*A(x)).
 * https://oeis.org/A218251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binom(n-2k, k) * binom(2n-4k+3, n-3k+1) /
 *            (2n-4k+3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t mul_div(seq_t x, seq_t y, idx_t d) {
#if defined(__SIZEOF_INT128__) || defined(_INT128)
    unsigned __int128 prod = (unsigned __int128)x * (unsigned __int128)y;
    return (seq_t)(prod / (unsigned __int128)d);
#else
    return (x * y) / d;   /* safe for small seq_t types (uint8_t, uint16_t) */
#endif
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n_minus_2k = n - 2 * k;
        idx_t n_minus_3k_plus1 = n - 3 * k + 1;
        idx_t denom = 2 * n - 4 * k + 3;  /* > 0 */

        seq_t t1 = binom(n_minus_2k, k);
        seq_t t2 = binom(denom, n_minus_3k_plus1);
        seq_t term = mul_div(t1, t2, denom);

        sum += term;
    }
    return sum;
}
