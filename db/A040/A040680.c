/*
 * A040680  Continued fraction for sqrt(707).
 * https://oeis.org/A040680
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

/* Periodic continued fraction for √707.
   a(0) = floor(√707) = 26.
   The continued fraction repeats the block [1,1,2,3,2,1,1,52] afterwards. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the n‑th term of the continued fraction expansion of √707. */
seq_t a(idx_t n)
{
    /* Periodic coefficients: a(0) followed by the repeating block. */
    static seq_t period[64];
    static size_t period_len = 0;  /* number of stored elements (a(0)+period) */
    static bool initialized = false;

    /* Initialise the period once. */
    if (!initialized) {
        const uint64_t D = 707;          /* square‑free part */
        const uint64_t a0 = 26;          /* floor(√D) */

        /* First coefficient */
        period[0] = (seq_t)a0;
        period_len = 1;

        /* Variables used in the standard algorithm. */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0;

        /* Generate coefficients until the last term of the period (2·a0). */
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0)                /* period finished */
                break;
        }
        /* period_len now equals 1 + period length. */
        initialized = true;
    }

    /* Return the requested term. */
    if (n < period_len)                               /* term stored directly */
        return period[(size_t)n];

    /* For n ≥ period_len use the repeating block (exclude a(0)). */
    size_t block_len = period_len - 1;                /* length of the period */
    size_t idx = ((size_t)(n - 1) % block_len) + 1;   /* index in period array */
    return period[idx];
}
