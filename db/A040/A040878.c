/*
 * A040878  Continued fraction for sqrt(909).
 * https://oeis.org/A040878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth partial quotient in the continued fraction
 *            expansion of sqrt(909)
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static const seq_t D = 909;
    static seq_t a0 = 0;
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static size_t period_cap = 0;

    if (period_len == 0) {
        a0 = floor_sqrt(D);

        period_cap = 16;
        period = seq_alloc(period_cap);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        while (1) {
            seq_t m_next = d * a - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            if (period_len == period_cap) {
                period_cap <<= 1;
                period = seq_realloc(period, period_len, period_cap);
            }
            period[period_len++] = a_next;

            if (a_next == 2 * a0) {
                break;
            }
            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    if (n == 0) {
        return a0;
    }

    idx_t pos = (n - 1) % period_len;
    return period[(size_t)pos];
}
