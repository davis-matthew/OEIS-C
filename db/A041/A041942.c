/*
 * A041942  Numerators of continued fraction convergents to sqrt(494).
 * https://oeis.org/A041942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(494)
 * a(0) = 22, a(1) = 89, a(2) = 200, ...
 * Recurrence: let a0 = ⌊√494⌋ = 22.
 * For k ≥ 0 define
 *   m0 = 0, d0 = 1
 *   m_{k+1} = d_k · a_k – m_k
 *   d_{k+1} = (494 – m_{k+1}²) / d_k
 *   a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋
 *   p_{-2} = 0, p_{-1} = 1
 *   p_k   = a_k · p_{k‑1} + p_{k‑2}
 * The sequence a(n) is p_n.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    // Constants for sqrt(494)
    const idx_t D   = 494;
    const idx_t a0  = 22;

    // Previous two numerators: p_{k-1} (prev1) and p_{k-2} (prev2)
    seq_t prev2 = (seq_t)0;   // p_{-2}
    seq_t prev1 = (seq_t)1;   // p_{-1}
    seq_t curr  = 0;

    // Continued fraction parameters
    idx_t m = 0;   // m_k
    idx_t d = 1;   // d_k
    idx_t a_k = a0; // a_k

    for (idx_t k = 0; k <= n; ++k)
    {
        // Compute p_k = a_k * p_{k-1} + p_{k-2}
        curr = (seq_t)a_k * prev1 + prev2;
        if (k == n)
            return curr;

        // Shift for next iteration
        prev2 = prev1;
        prev1 = curr;

        // Compute next a_{k+1}
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;  // floor division

        // Update parameters
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* Unreachable: return 0 to satisfy compiler */
    return (seq_t)0;
}
