/*
 * A080496  a(n) = A080495(n+1)/A080495(n).
 * https://oeis.org/A080496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = A080495(n+1)/A080495(n), where A080495(1)=1,
 *            A080495(2)=12, and A080495(k) is the smallest multiple of
 *            A080495(k-1) (multiplier >= 2) beginning with the last decimal
 *            digit of A080495(k-1); computed by search
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A080496_leading_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t cur = (seq_t)1;
    seq_t r = (seq_t)12;
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            r = (seq_t)12;
        } else {
            seq_t d = cur % (seq_t)10;
            if (d == (seq_t)0) return (seq_t)0;
            seq_t m = (seq_t)2;
            while (A080496_leading_digit(cur * m) != d) {
                ++m;
            }
            r = m;
        }
        cur = cur * r;
    }
    return r;
}
