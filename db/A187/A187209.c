/*
 * A187209  Sum of all terms of triangle of absolute differences of the
 *          divisors of n.
 * https://oeis.org/A187209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of all entries in the pyramid formed by successive
 *            absolute differences of the sorted divisors of n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ua = *(const uint64_t *)a;
    uint64_t ub = *(const uint64_t *)b;
    return (ua > ub) - (ua < ub);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* ---------- collect all divisors of n ---------- */
    uint64_t divs[256];
    size_t dcnt = 0;
    idx_t i;
    for (i = 1; (idx_t)i * (idx_t)i <= n; ++i)
    {
        if (n % i == 0)
        {
            divs[dcnt++] = (uint64_t)i;
            idx_t j = n / i;
            if (j != i) divs[dcnt++] = (uint64_t)j;
        }
    }

    /* ---------- sort the divisors ---------- */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* ---------- build the difference pyramid and sum ---------- */
    seq_t sum = 0;
    if (dcnt <= 1) return sum;

    uint64_t cur[256];
    size_t cur_len = dcnt - 1;
    size_t k;
    for (k = 0; k < cur_len; ++k)
    {
        cur[k] = divs[k + 1] - divs[k];
        sum += (seq_t)cur[k];
    }

    while (cur_len > 1)
    {
        size_t next_len = cur_len - 1;
        for (k = 0; k < next_len; ++k)
        {
            uint64_t a = cur[k];
            uint64_t b = cur[k + 1];
            uint64_t diff = (a > b) ? a - b : b - a;
            sum += (seq_t)diff;
            cur[k] = diff;
        }
        cur_len = next_len;
    }

    return sum;
}
