/*
 * A033898  Sort then Add, a(1)=15.
 * https://oeis.org/A033898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=15; a(n)=a(n-1)+sort_digits_ascending(a(n-1)) for n>1
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 62, u64 n <= 327,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 993 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t sort_digits_ascending(seq_t x)
{
    unsigned int cnt[10] = {0};
    seq_t y = x;
    while (y > 0) {
        unsigned int d = (unsigned int)(y % 10);
        ++cnt[d];
        y = y / 10;
    }
    seq_t res = 0;
    for (int d = 0; d <= 9; ++d) {
        for (unsigned int k = 0; k < cnt[d]; ++k) {
            res = res * 10 + d;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)15;
    seq_t val = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i)
        val += sort_digits_ascending(val);
    return val;
}
