/*
 * A006116  Sum of Gaussian binomial coefficients [n,k] for q=2 and k=0..n.
 * https://oeis.org/A006116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = 2*a(n) + (2^n - 1)*a(n-1), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

seq_t a(idx_t n) {
    seq_t prev = 1, cur = 2, p = 2, next, t;
    idx_t m;
    int big = seq_is_big();
    if (n == 0) return prev;
    for (m = 1; m < n; m++) {
        if (big)
            t = (prev << m) - prev;
        else
            t = prev * p - prev;
        next = cur * 2 + t;
        prev = cur;
        cur = next;
        p = p * 2;
    }
    return cur;
}
