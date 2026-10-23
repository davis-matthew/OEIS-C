/*
 * A040324  Continued fraction for sqrt(343).
 * https://oeis.org/A040324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction coefficients of sqrt(343) (offset 0) */
/* a(0) = floor(sqrt(343)) = 18; thereafter periodic coefficients. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic data for the continued fraction of sqrt(343). */
    static uint64_t a0 = 0;         /* integer part floor(sqrt(343)) */
    static uint64_t *period = NULL;/* coefficients after a0 */
    static size_t period_len = 0;   /* length of the period */
    static bool init = false;      /* one‑time initialization flag */

    if (!init)
    {
        /* ---------- compute floor(sqrt(343)) ------------------- */
        const uint64_t N = 343ULL;
        uint64_t low = 0, high = N, root = 0;
        while (low <= high)
        {
            uint64_t mid = (low + high) >> 1;
            uint64_t sq = mid * mid;
            if (sq == N) { root = mid; break; }
            if (sq < N) { root = mid; low = mid + 1; }
            else { high = mid - 1; }
        }
        a0 = root;

        /* ---------- compute the periodic part ----------------- */
        uint64_t m = 0, d = 1, a = a0;
        const uint64_t two_a0 = 2 * a0;

        /* small capacity is enough for this small period */
        size_t cap = 16;
        period = (uint64_t *)malloc(sizeof(uint64_t) * cap);
        if (!period) {
            init = true; /* allocation failure – return a0 only */
            return (seq_t)a0;
        }

        while (1)
        {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;

            if (period_len == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(period, sizeof(uint64_t) * cap);
                if (!tmp) {
                    free(period);
                    period = NULL;
                    init = true;
                    return (seq_t)a0;
                }
                period = tmp;
            }
            period[period_len++] = a;

            if (a == two_a0) break;   /* period ends */
        }

        init = true;
    }

    if (n == 0) return (seq_t)a0;

    if (period_len == 0) return (seq_t)a0; /* fallback on allocation failure */

    size_t idx = (size_t)((n - 1) % period_len);
    return (seq_t)period[idx];
}
