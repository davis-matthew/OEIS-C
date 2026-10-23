/*
 * A040918  Continued fraction for sqrt(949).
 * https://oeis.org/A040918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040918: Continued fraction terms of sqrt(949).
   a(0) = floor(sqrt(949)) = 30
   a(n+1) = floor((a0 + m_{n+1}) / d_{n+1})  (n ≥ 0)
   m_{n+1} = d_n * a_n - m_n
   d_{n+1} = (D - m_{n+1}^2) / d_n
   a0 = floor(sqrt(D)),  D = 949.
   This implementation recomputes the recurrence from scratch
   up to the requested index n, avoiding dynamic allocation and
   ensuring compatibility with arbitrary‑precision seq_t. */
#include <stdint.h>
#include <stdbool.h>

static const seq_t D_val = 949;   /* D = 949 */

/* Return the n-th term of the continued‑fraction expansion of sqrt(949). */
seq_t a(idx_t n)
{
    /* Compute a0 = floor(sqrt(D)).  D is small, so a simple loop works. */
    seq_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D_val)
        ++a0;

    /* Initial values of the recurrence. */
    seq_t m = 0;          /* m0 */
    seq_t d = 1;          /* d0 */
    seq_t a_cur = a0;     /* a0 */

    if (n == 0)
        return a_cur;

    /* Iterate up to index n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D_val - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Advance state for next iteration. */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return a_cur;
}
