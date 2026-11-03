/*
 * A007224  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+3,n) punctured convolutional codes from (2,1).
 * https://oeis.org/A007224
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(2*n, n+3) + 2*[(n mod 3) ==
 *            0]*binomial(2*n/3, n/3 + 1))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 27 (2026-11-01)
 * Helpers:   binomial, binomial_u64_to_u128, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/binomial_u64_to_u128.h"
#include "../utils/gcd_u64.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t s = binomial(2 * n, n + 3);
        if (n % 3 == 0) {
            s += 2 * binomial(2 * n / 3, n / 3 + 1);
        }
        return s / (seq_t)n;
    } else {
        unsigned __int128 s = binomial_u64_to_u128(2 * n, n + 3);
        if (n % 3 == 0) {
            s += 2 * binomial_u64_to_u128(2 * n / 3, n / 3 + 1);
        }
        return u128_to_seq(s / n);
    }
}
