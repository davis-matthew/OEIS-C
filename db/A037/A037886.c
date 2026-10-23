/*
 * A037886  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*9^i} is the base 9
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d(i)-e(i)| where n = Σ d(i) 9^i (base‑9
 *            expansion of n) and e(i) are the digits d(i) sorted in
 *            nondecreasing order. a(1) = 0, a(2) = 0, … (offset 1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Sort an array of unsigned 8‑bit integers in ascending order */
static void sort_uint8(uint8_t *a, size_t len)
{
    for (size_t i = 0; i + 1 < len; ++i)
    {
        for (size_t j = i + 1; j < len; ++j)
        {
            if (a[i] > a[j])
            {
                uint8_t tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Base‑9 digits of n (least significant first) */
    uint8_t d[32];   /* max digits for 64‑bit n is 21, 32 is safe */
    size_t len = 0;
    idx_t tmp = n;

    /* Handle n == 0 just in case – not needed for offset 1 */
    if (tmp == 0)
    {
        d[0] = 0;
        len = 1;
    }
    else
    {
        while (tmp > 0)
        {
            d[len++] = (uint8_t)(tmp % 9);
            tmp /= 9;
        }
    }

    /* Copy and sort to obtain e(i) */
    uint8_t e[32];
    for (size_t i = 0; i < len; ++i)
        e[i] = d[i];
    sort_uint8(e, len);

    /* Compute half the sum of absolute differences */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t diff = (d[i] > e[i]) ? (d[i] - e[i]) : (e[i] - d[i]);
        sum += (seq_t)diff;
    }

    return sum / 2;
}
