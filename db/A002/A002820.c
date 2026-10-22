/*
 * A002820  Number of n X n invertible binary matrices A such that A+I is
 *          invertible.
 * https://oeis.org/A002820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^(n(n-1)/2) * A005327(n+1), where A005327(n+1) = S(n)
 *            with S(n) = (2^n - 1)*S(n-1) + (-1)^n, S(0) = 1. Equivalently
 *            a(n) = 2^(n(n-1)/2) * Sum_{k=0..n} (-1)^k * Prod_{i=k+1..n} (2^i
 *            - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   a002820_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002820_pow2.h"

seq_t a(idx_t n) {
    seq_t s = 1;
    seq_t p = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        p = p * 2;
        s = s * p - s;
        if (i & 1) s = s - 1;
        else s = s + 1;
    }
    idx_t e = (n == 0) ? 0 : (n / 2) * (n - 1) + ((n & 1) ? 0 : 0);
    /* e = n*(n-1)/2 computed without overflow concerns */
    if (n % 2 == 0) e = (n / 2) * (n - 1);
    else e = n * ((n - 1) / 2);
    return s * a002820_pow2(e);
}
