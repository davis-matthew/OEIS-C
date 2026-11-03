/*
 * A040447  Continued fraction for sqrt(469).
 * https://oeis.org/A040447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = coefficient n in the continued‑fraction expansion of √469
 *            Offset: 0 (a(0) is the first term). The sequence is periodic
 *            after the first term with period 10: 1,1,1,10,6,10,1,1,1,42
 * Range:     u8 none, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    static seq_t period[20];
    static size_t period_len = 0;
    static uint64_t a0 = 0;

    /* Initialise once – compute the period of the continued fraction. */
    if (period_len == 0) {
        a0 = (uint64_t)floor_sqrt((seq_t)469);

        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t next = a0;
        size_t i = 0;

        /* Generate terms a1, a2, … until a_k = 2*a0 (end of period). */
        do {
            m = d * next - m;
            d = (469 - m * m) / d;
            next = (a0 + m) / d;
            period[i++] = (seq_t)next;
        } while (next != 2 * a0);

        period_len = i;
    }

    if (n == 0)
        return (seq_t)a0;

    /* a(1) corresponds to period[0], a(2) to period[1], … */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
