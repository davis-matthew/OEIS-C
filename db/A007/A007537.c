/*
 * A007537  Number of proper covers of an n-set.
 * https://oeis.org/A007537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000371(n) - 2^(2^n-1))/2 = Sum_{k=0..n-1}
 *            (-1)^k*C(n,k)*2^(2^(n-k)-1) + (-1)^n - 2^(2^n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binom, pw2, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pw2.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t a7537_p2(idx_t e) {
    if (seq_is_fixed_width()) {
        if (e >= (idx_t)(8 * sizeof(seq_t))) return (seq_t)0;
        return ((seq_t)1) << e;
    }
    return pw2(e);
}

seq_t a(idx_t n) {
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    idx_t k;
    /* fixed-width builds: a(n) for n >= 8 exceeds 128 bits */
    if (seq_is_fixed_width() && n >= 8) return (seq_t)0;
    if (n >= 63) return (seq_t)0; /* astronomically large, unreachable */
    for (k = 0; k < n; k++) {
        idx_t m = n - k;
        idx_t e = (((idx_t)1) << m) - 1;
        seq_t term = binom(n, k) * a7537_p2(e);
        if (k % 2 == 0) pos += term; else neg += term;
    }
    if (n % 2 == 0) pos += (seq_t)1; else neg += (seq_t)1;
    neg += a7537_p2((((idx_t)1) << n) - 2);
    return pos - neg;
}
