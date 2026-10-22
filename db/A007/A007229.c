/*
 * A007229  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+2,n) punctured convolutional codes from (4,1).
 * https://oeis.org/A007229
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(4*n, n+2) + [(n mod 2) ==
 *            0]*binomial(2*n, n/2 + 1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t gcd_u128(u128_t x, u128_t y) {
    while (y != 0) {
        u128_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* exact binomial in 128 bits, reducing by gcd so intermediates never exceed the result */
static u128_t binom_u128(uint64_t m, uint64_t k) {
    if (k > m) return 0;
    if (k > m - k) k = m - k;
    u128_t c = 1;
    for (uint64_t i = 1; i <= k; i++) {
        u128_t num = (u128_t)(m - k + i);
        u128_t g = gcd_u128(c, (u128_t)i);
        c = (c / g) * (num / ((u128_t)i / g));
    }
    return c;
}

static seq_t binom_big(idx_t m, idx_t k) {
    if (k > m) return 0;
    if (k > m - k) k = m - k;
    seq_t c = 1;
    for (idx_t i = 1; i <= k; i++) {
        c *= (seq_t)(m - k + i);
        c /= (seq_t)i;
    }
    return c;
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t sum = binom_big(4 * n, n + 2);
        if (n % 2 == 0) sum += binom_big(2 * n, n / 2 + 1);
        return sum / (seq_t)n;
    } else {
        u128_t sum = binom_u128(4 * n, n + 2);
        if (n % 2 == 0) sum += binom_u128(2 * n, n / 2 + 1);
        return u128_to_seq(sum / (u128_t)n);
    }
}
