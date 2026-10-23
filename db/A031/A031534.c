/*
 * A031534  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 36.
 * https://oeis.org/A031534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the continued fraction * of
 *            sqrt(k) has even period, and after deleting the last term of the
 *            * period the central term equals 36. Computed by exhaustive
 *            search
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Test whether a given k satisfies the conditions of the sequence. */
static bool qualifies(uint64_t k)
{
    if (k <= 1) return false;

    uint64_t a0 = floor_sqrt_u64(k);
    if (a0 * a0 == k) return false;

    /* Dynamic buffer for the period terms.  Keep it between calls. */
    static uint64_t *period = NULL;
    static size_t period_cap = 0;
    size_t period_len = 0;

    /* Ensure enough space for at least 4 terms to begin with. */
    if (period_cap < 4) {
        period_cap = 8;
        period = (uint64_t *)realloc(period, period_cap * sizeof(uint64_t));
    }

    uint64_t m = 0, d = 1, a = a0;
    while (1) {
        uint64_t m_next = d * a - m;
        __int128 num = (__int128)k - (__int128)m_next * m_next;
        uint64_t d_next = (uint64_t)(num / d);
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Grow the buffer if necessary. */
        if (period_len >= period_cap) {
            period_cap <<= 1;
            period = (uint64_t *)realloc(period, period_cap * sizeof(uint64_t));
        }

        period[period_len++] = a_next;

        if (a_next == 2 * a0) break;   /* last term of the period */

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* Even period length and central term after deleting last element. */
    if (period_len % 2 != 0) return false;

    /* Central term of the truncated period (length period_len-1). */
    size_t central_index = (period_len - 1) / 2;
    if (period[central_index] == 36) return true;
    return false;
}

/* The requested OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset 1 */

    /* Memoised list of found values. */
    static seq_t *list = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;
    static uint64_t last_k = 1;

    while (len < n) {
        uint64_t k = ++last_k;
        if (qualifies(k)) {
            if (len >= cap) {
                cap = cap ? cap * 2 : 8;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = (seq_t)k;
        }
    }
    return list[n - 1];
}
