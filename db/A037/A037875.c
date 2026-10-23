/*
 * A037875  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*7^i} is base 7
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d(i) – e(i)|, where the base‑7 digits d(i) of
 *            n are sorted into non‑increasing order to give the digits e(i)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static int cmp_desc(const void *a, const void *b)
{
    uint8_t va = *(const uint8_t*)a;
    uint8_t vb = *(const uint8_t*)b;
    return (va < vb) ? 1 : (va > vb) ? -1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint8_t orig[64];
    uint8_t sorted[64];
    size_t len = 0;
    idx_t tmp = n;

    /* Extract base‑7 digits (least significant first) */
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 7);
        orig[len] = d;
        sorted[len] = d;
        ++len;
        tmp /= 7;
    }

    /* Sort digits into non‑increasing order */
    qsort(sorted, len, sizeof(uint8_t), cmp_desc);

    /* Sum absolute differences at each position */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        int diff = (int)orig[i] - (int)sorted[i];
        if (diff < 0)
            diff = -diff;
        sum += (seq_t)diff;
    }

    return sum / 2;
}
