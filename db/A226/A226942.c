/*
 * A226942  Number of iterations of A226911 until 0 is reached, for starting
 *          value n.
 * https://oeis.org/A226942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 1 + a(A226911(n)), a(0) = 0, where A226911(n) = (sum of
 *            letter values of the English word for n) mod n; the code
 *            iterates this map until 0 and counts the steps
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9811 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t A226942_units[10] = {0, 34, 58, 56, 60, 42, 52, 65, 49, 42};
static const uint64_t A226942_teens[10] = {39, 63, 87, 99, 104, 65, 96, 109, 73, 86};
static const uint64_t A226942_tens[10] = {0, 0, 107, 100, 84, 66, 97, 110, 74, 87};
static const uint64_t A226942_scale[7] = {0, 102, 75, 73, 109, 132, 167};

static uint64_t A226942_group(uint64_t g)
{
    uint64_t t = 0;
    if (g >= 100) {
        t += A226942_units[g / 100] + 74;
        g %= 100;
    }
    if (g >= 20) {
        t += A226942_tens[g / 10];
        g %= 10;
        if (g > 0) t += A226942_units[g];
    } else if (g >= 10) {
        t += A226942_teens[g - 10];
    } else if (g > 0) {
        t += A226942_units[g];
    }
    return t;
}

static uint64_t A226942_step(uint64_t n)
{
    uint64_t grp[7];
    size_t k = 0, i;
    uint64_t t = n, total = 0;
    if (n == 0) return 0;
    while (t > 0 && k < 7) {
        grp[k++] = t % 1000;
        t /= 1000;
    }
    for (i = k; i-- > 0; ) {
        if (grp[i] == 0) continue;
        total += A226942_group(grp[i]);
        total += A226942_scale[i];
    }
    return total % n;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned long long cnt = 0;
    while (m > 0) {
        m = A226942_step(m);
        cnt++;
    }
    return (seq_t)cnt;
}
