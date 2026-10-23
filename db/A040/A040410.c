/*
 * A040410  Continued fraction for sqrt(431).
 * https://oeis.org/A040410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040410: continued fraction coefficients of sqrt(431).
   a(0) = floor(sqrt(431)) = 20.
   For n ≥ 1, a(n) is the (n‑1)‑th term of the periodic part:
   [1,3,5,1,2,7,1,19,1,7,2,1,5,3,1,40] repeating. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction */
    static bool          initialized = false;
    static seq_t         a0;          /* floor(sqrt(431)) */
    static seq_t         period[64];  /* enough for this D */
    static uint64_t      period_len;

    if (!initialized)
    {
        /* compute a0 with a type large enough to avoid overflow */
        uint64_t D = 431;
        uint64_t a0_u = 0;
        while ((a0_u + 1) * (a0_u + 1) <= D)
            ++a0_u;
        a0 = (seq_t)a0_u;

        /* continued‑fraction iteration, using 64‑bit temporaries */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0_u;
        period_len = 0;

        for (;;)
        {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0_u + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0_u)
                break;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* index into the periodic part (0‑based) */
    return period[(size_t)((n - 1) % period_len)];
}
