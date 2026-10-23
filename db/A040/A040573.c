/*
 * A040573  Continued fraction for sqrt(598).
 * https://oeis.org/A040573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; a(n)=period[(n-1)%period_len] for n>=1, where period is
 *            the repeating part of the continued fraction of sqrt(598)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Persistent state for the continued fraction of sqrt(598) */
    static bool init = false;
    static seq_t a0;
    static seq_t period[20];
    static idx_t period_len;

    if (!init) {
        /* integer part of sqrt(598) */
        a0 = (seq_t)24;                /* 24 fits into any unsigned integer type */

        /* Compute the period using standard continued‑fraction algorithm.
           All intermediate values are kept in 64‑bit unsigned integers
           to avoid overflow even when seq_t is only 8 bits. */
        const uint64_t S = 598ULL;
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = (uint64_t)24;
        idx_t k = 0;

        do {
            /* m = d*a - m */
            m = d * a - m;
            /* d = (S - m*m) / d */
            d = (S - m * m) / d;
            /* a = (a0 + m) / d */
            a = ((uint64_t)24 + m) / d;

            period[k++] = (seq_t)a;
        } while (a != 2 * (uint64_t)24);

        period_len = k;
        init = true;
    }

    if (n == 0)
        return a0;

    /* Periodic part starts at index 1 */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
