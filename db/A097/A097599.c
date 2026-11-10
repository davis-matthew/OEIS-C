/*
 * A097599  Differences between A097598 and A047842.
 * https://oeis.org/A097599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = |A097598(n) - A047842(n)|
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A097598_a(idx_t n)
{
    if (n == 0)
        return 10;
    seq_t result = 0;
    idx_t temp = n;
    while (temp > 0) {
        int digit = (int)(temp % 10);
        idx_t cnt = 0;
        while (temp > 0 && (int)(temp % 10) == digit) {
            cnt++;
            temp /= 10;
        }
        result = result * 100 + (seq_t)(cnt * 10 + digit);
    }
    return result;
}

static seq_t A047842_a(idx_t n)
{
    unsigned int cnt[10] = {0};
    idx_t t = n;
    if (t == 0)
        cnt[0] = 1;
    else
        while (t > 0) {
            cnt[t % 10]++;
            t /= 10;
        }
    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        unsigned int c = cnt[d];
        if (c == 0)
            continue;
        int digits_c = (c < 10) ? 1 : (c < 100 ? 2 : 3);
        seq_t mult = 1;
        for (int i = 0; i < digits_c + 1; ++i)
            mult *= (seq_t)10;
        seq_t part = ((seq_t)c) * 10 + (seq_t)d;
        result = result * mult + part;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = A097598_a(n);
    seq_t y = A047842_a(n);
    return (x >= y) ? (seq_t)(x - y) : (seq_t)(y - x);
}
