/*
 * A040825  Continued fraction for sqrt(855).
 * https://oeis.org/A040825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(855)).  
   For n ≥ 1, a(n) is obtained by the continued‑fraction algorithm
   for sqrt(855).  
   Let D = 855, a0 = floor(sqrt(D)).  Start with m = 0, d = 1, a = a0.
   For each step:
       m' = d * a - m
       d' = (D - m' * m') / d
       a' = (a0 + m') / d'
   After n steps, a holds a(n).  The returned value fits in seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constant D = 855 */
    const uint64_t D = 855ULL;

    /* compute a0 = floor(sqrt(D)) with integer arithmetic */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) {
        ++a0;
    }

    if (n == 0) {
        return (seq_t)a0;
    }

    /* iterate the continued‑fraction recurrence n times */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_val = a0;

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return (seq_t)a_val;
}
