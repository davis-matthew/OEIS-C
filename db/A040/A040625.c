/*
 * A040625  Continued fraction for sqrt(651).
 * https://oeis.org/A040625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the nth term of the simple continued fraction for
 *            sqrt(651). a(0) = ⌊√651⌋ = 25 For n ≥ 1 the terms repeat the
 *            period [1, 1, 16, 1, 1, 50] of length 6
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute ⌊√x⌋ for 0 ≤ x < 2^64 using integer arithmetic. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* ------------------------------------------------------------------ */
/* Cached period data                                            */
/* ------------------------------------------------------------------ */
static seq_t a0 = 0;          /* first term ⌊√651⌋                                                     */
static seq_t period[64] = {0};/* period of the continued fraction                                   */
static size_t period_len = 0; /* actual length of the period (should be 6)                           */
static int initialized = 0;   /* 0 → not yet initialized, 1 → ready                            */

/* Initialise a0 and period. */
static void init_period(void)
{
    const idx_t D = 651;                      /* discriminant */
    idx_t a0_i = floor_sqrt_u64(D);           /* integer part of √D */
    a0 = (seq_t)a0_i;                         /* store as seq_t */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0_i;
    size_t pos = 0;

    /* Generate the continued‑fraction terms until the period completes. */
    while (1) {
        /* m_{k+1} = d_k * a_k - m_k */
        m = d * a - m;
        /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        d = (D - m * m) / d;
        /* a_{k+1} = (a0 + m_{k+1}) / d_{k+1} */
        a = (a0_i + m) / d;

        period[pos++] = (seq_t)a;

        /* Period ends when a_{k+1} = 2*a0. */
        if (a == 2 * a0_i)
            break;
    }

    period_len = pos;
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Public API                                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized)
        init_period();

    if (n == 0)
        return a0;

    /* All terms from n = 1 onward are periodic. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
