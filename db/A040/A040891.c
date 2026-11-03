/*
 * A040891  Continued fraction for sqrt(922).
 * https://oeis.org/A040891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(922)
   a(0) = floor(sqrt(922))
   a(n) = period[(n‑1) mod period_len] for n ≥ 1
   Period terminates when a == 2·a(0)   */

/* Note: 922 is a constant, so all arithmetic fits in idx_t.  */

#include <stdbool.h>
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

static bool initialized = false;
static seq_t a0;                      /* a(0) – the preperiod term          */
static seq_t period[128];             /* the repeating part                 */
static idx_t period_len;              /* length of period                    */

static void init(void)
{
    /* constants as idx_t (they fit easily) */
    const idx_t D = 922;                /* non‑square rational in question    */
    a0 = floor_sqrt((seq_t)D);          /* a0 as seq_t                        */
    const idx_t a0i = (idx_t)a0;        /* same value as idx_t                */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0i;

    period_len = 0;
    while (1) {
        /* m_{k+1} = d_k * a_k – m_k */
        m = d * a - m;
        /* d_{k+1} = (D – m_{k+1}^2) / d_k   (integer division) */
        d = (D - m * m) / d;
        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        a = (a0i + m) / d;

        period[period_len++] = (seq_t)a;
        if (a == a0i * 2) break;        /* period ends here                  */
    }

    initialized = true;
}

/* Public function: returns a(n) for any n >= 0, independent of order of calls. */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return a0;
    return period[(n - 1) % period_len];
}
