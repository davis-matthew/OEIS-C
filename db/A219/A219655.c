/*
 * A219655  Greatest inverse of A219652; a(n) = maximal i such that A219652(i)
 *          = n.
 * https://oeis.org/A219655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal i with A219652(i) = n; computed by tabulating
 *            A219652(i) = 1 + A219652(i - s(i)), where s is the
 *            factorial-base digit sum, until it exceeds n (A219652 is
 *            nondecreasing), then taking the last index with value n
 * Range:     u8 n <= 52, u16 n <= 5154, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 74 terms through n = 9307 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A219655_st = 0;
static size_t A219655_cnt = 0, A219655_cap = 0;

static idx_t A219655_fds(idx_t v)
{
    idx_t sum = 0, d = v, i = 1;
    while (d > 0) {
        idx_t div = i + 1;
        sum += d % div;
        d /= div;
        ++i;
    }
    return sum;
}

static void A219655_push(void)
{
    if (A219655_cnt == 0) {
        A219655_cap = 1024;
        A219655_st = (uint32_t *)malloc(A219655_cap * sizeof(uint32_t));
        A219655_st[0] = 0;
        A219655_cnt = 1;
        return;
    }
    if (A219655_cnt == A219655_cap) {
        A219655_cap *= 2;
        A219655_st = (uint32_t *)realloc(A219655_st, A219655_cap * sizeof(uint32_t));
    }
    idx_t i = (idx_t)A219655_cnt;
    A219655_st[i] = 1 + A219655_st[i - A219655_fds(i)];
    A219655_cnt++;
}

seq_t a(idx_t n)
{
    if (A219655_cnt == 0) A219655_push();
    while ((idx_t)A219655_st[A219655_cnt - 1] <= n) A219655_push();
    /* binary search: first index with st > n */
    size_t lo = 0, hi = A219655_cnt - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((idx_t)A219655_st[mid] > n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)(lo - 1);
}
