/*
 * A040773  Continued fraction for sqrt(802).
 * https://oeis.org/A040773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Continued fraction expansion of sqrt(802).
 * a(0) is the integer part: floor(sqrt(802)) = 28.
 * The subsequent terms repeat periodically with period length 12:
 *   3,7,1,3,6,28,6,3,1,7,3,56
 * The sequence is computed lazily on first call and then reused.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction after the integer part. */
    static seq_t period[12];
    static size_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t N = 802;
        /* Compute floor(sqrt(N)) by integer search */
        idx_t a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= N)
            ++a0;

        /* Generate the continued‑fraction terms until the last term 2*a0 = 56 is reached */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_term = a0;

        size_t i = 0;
        do {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a_term - m;
            /* d_{k+1} = (N - m_{k+1}^2) / d_k */
            d = (N - m * m) / d;
            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
            a_term = (a0 + m) / d;
            period[i++] = (seq_t)a_term;
        } while (a_term != 2 * a0);   /* a_{k+1} == 2*a0 terminates the period */

        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return (seq_t)28;                     /* integer part */
    idx_t k = (n - 1) % period_len;           /* periodic indexing */
    return period[(size_t)k];
}
