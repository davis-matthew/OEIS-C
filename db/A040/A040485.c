/*
 * A040485  Continued fraction for sqrt(508).
 * https://oeis.org/A040485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n-th partial quotient of the continued fraction for
 *            √508. Recurrence: a(0) = floor(√508) = 22 m₀ = 0, d₀ = 1 for k ≥
 *            0: m_{k+1} = d_k * a_k – m_k d_{k+1} = (508 – m_{k+1}²) / d_k
 *            a_{k+1} = floor((a₀ + m_{k+1}) / d_{k+1}) (All arithmetic is
 *            performed on seq_t. For seq_t as an arbitrary‑...
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n) {
    const seq_t D = (seq_t)508;      /* value under the square root        */
    const seq_t a0 = floor_sqrt(D);  /* floor(√508) = 22                    */

    if (n == 0) return a0;

    /* State variables for the recurrence */
    seq_t m = 0;        /* m_k   */
    seq_t d = 1;        /* d_k   */
    seq_t a_val = a0;   /* a_k   */

    /* Iterate until the n-th term is reached */
    for (idx_t k = 0; k < n; ++k) {
        seq_t m_next = d * a_val - m;           /* m_{k+1} */
        seq_t d_next = (D - m_next * m_next) / d; /* d_{k+1} */
        seq_t a_next = (a0 + m_next) / d_next;     /* a_{k+1} */

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return a_val;   /* a_n */
}
