/*
 * A040441  Continued fraction for sqrt(463).
 * https://oeis.org/A040441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction coefficients of √463 a(0) = ⌊√463⌋ = 21; a(n)
 *            for n ≥ 1 are the periodic part
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t a0;
    static seq_t period[512];
    static idx_t period_len = 0;

    if (!init) {
        /* Compute a0 = floor(√463) */
        int a0int = 0;
        while ((a0int + 1) * (a0int + 1) <= 463) {
            ++a0int;
        }
        a0 = (seq_t)a0int;

        /* Generate the periodic part of the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;
        size_t idx = 0;
        const seq_t target = a0 * 2;       /* termination condition */

        do {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a_curr - m;

            /* d_{k+1} = (463 - m_{k+1}^2) / d_k  (use 64‑bit to avoid overflow) */
            uint64_t m_sq = (uint64_t)m * (uint64_t)m;
            uint64_t numerator = 463 - m_sq;
            d = (seq_t)(numerator / d);

            /* a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋ */
            a_curr = (a0 + m) / d;

            period[idx++] = a_curr;
        } while (a_curr != target);

        period_len = (idx_t)idx;
        init = true;
    }

    if (n == 0) {
        return a0;
    }

    /* For n ≥ 1: return periodic part element */
    idx_t k = (n - 1) % period_len;
    return period[(size_t)k];
}
