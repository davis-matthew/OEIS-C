/*
 * A037885  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*8^i} is the base 8
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*Sum(|d(i)-e(i)|) where d(i) are the base-8 digits
 *            of n and e(i) are those digits sorted in nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_uint8(const void *a, const void *b)
{
    return (*(uint8_t *)a - *(uint8_t *)b);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* maximum base-8 digits for 64‑bit unsigned integer is 22;
     * allocate a bit more to be safe. */
    uint8_t d[64];
    uint8_t e[64];
    idx_t t = n;
    size_t len = 0;

    /* extract digits least‑significant first */
    while (t > 0) {
        d[len] = (uint8_t)(t & 7U);   /* t % 8 */
        t >>= 3;                      /* t /= 8 */
        ++len;
    }

    /* copy for sorting */
    for (size_t i = 0; i < len; ++i)
        e[i] = d[i];

    /* sort digits ascending */
    qsort(e, len, sizeof(uint8_t), cmp_uint8);

    /* compute sum of absolute differences */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t di = d[i];
        uint8_t ei = e[i];
        sum += (seq_t)(di > ei ? di - ei : ei - di);
    }

    /* divide by 2 – the sum is always even */
    return sum / (seq_t)2;
}
