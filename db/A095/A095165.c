/*
 * A095165  n divided by A095163(n).
 * https://oeis.org/A095165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A095163(n), where A095163(n) is the smallest divisor
 *            d of n that occurred earlier in A095163 fewer than d times
 *            (computed by direct search)
 * Range:     u8 n <= 593, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9354 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A095163_seq = NULL;
static size_t A095163_len = 0;
static size_t A095163_cap = 0;

static idx_t *A095163_cnt = NULL;
static size_t A095163_cnt_cap = 0;

static idx_t A095163_isqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

static void A095163_ensure_seq(size_t need)
{
    if (need <= A095163_cap)
        return;
    size_t new_cap = A095163_cap ? A095163_cap * 2 : 128;
    while (new_cap < need)
        new_cap <<= 1;
    A095163_seq = mem_resize(seq_t, A095163_seq, new_cap);
    A095163_cap = new_cap;
}

static void A095163_ensure_cnt(idx_t value)
{
    if ((size_t)value < A095163_cnt_cap)
        return;
    size_t new_cap = A095163_cnt_cap ? A095163_cnt_cap * 2 : 64;
    while (new_cap <= (size_t)value)
        new_cap <<= 1;
    idx_t *new_cnt = (idx_t *)realloc(A095163_cnt, new_cap * sizeof(idx_t));
    for (size_t i = A095163_cnt_cap; i < new_cap; ++i)
        new_cnt[i] = 0;
    A095163_cnt = new_cnt;
    A095163_cnt_cap = new_cap;
}

static seq_t A095163_a(idx_t n)
{
    if (n == 0)
        return 0;

    for (idx_t k = A095163_len + 1; k <= n; ++k) {
        idx_t limit = A095163_isqrt(k);
        idx_t best_d = k;

        for (idx_t d = 1; d <= limit; ++d) {
            if (k % d == 0) {
                A095163_ensure_cnt(d);
                if (A095163_cnt[d] < d && d < best_d)
                    best_d = d;

                idx_t other = k / d;
                if (other != d) {
                    A095163_ensure_cnt(other);
                    if (A095163_cnt[other] < other && other < best_d)
                        best_d = other;
                }
            }
        }

        A095163_ensure_seq(A095163_len + 1);
        A095163_seq[A095163_len++] = (seq_t)best_d;

        A095163_ensure_cnt(best_d);
        A095163_cnt[best_d]++;
    }

    return A095163_seq[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return (seq_t)n / A095163_a(n);
}
