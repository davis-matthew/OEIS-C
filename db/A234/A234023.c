/*
 * A234023  Positions n where abs(A234022(n)-A234022(n+1)) > 1.
 * https://oeis.org/A234023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with |A000120(A193231(k)) -
 *            A000120(A193231(k+1))| > 1; found by linear search over k
 * Range:     u8 n <= 12, u16 n <= 3168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9494 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A234023_blue(idx_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;
    idx_t t = n;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

static int A234023_pop(seq_t x)
{
    int c = 0;
    while (x != 0) {
        if ((int)(x & 1)) c++;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    int p = A234023_pop(A234023_blue(0));
    for (;;) {
        int q = A234023_pop(A234023_blue(k + 1));
        int d = p - q;
        if (d < 0) d = -d;
        if (d > 1) {
            count++;
            if (count == n) return (seq_t)k;
        }
        p = q;
        k++;
    }
}
