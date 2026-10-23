/*
 * A040499  Continued fraction for sqrt(522).
 * https://oeis.org/A040499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = continued fraction coefficient of sqrt(522) at index n
 *            where a(0) = floor(sqrt(522)) = 22. The algorithm generates the
 *            periodic sequence observed in the OEIS entry
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t is an unsigned integer of 8, 16, 32, 64, 128 or arbitrary-
   precision; idx_t is an unsigned 64‑bit integer. */

/* The computation uses only integer arithmetic (no floating point). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed constant for sqrt(522) */
    const unsigned long long D = 522ULL;
    const unsigned long long a0 = 22ULL;   /* floor(sqrt(522)) */

    if (n == 0)
        return (seq_t)a0;

    /* Variables for the continued fraction algorithm */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a_cur = a0;   /* current a_k */

    /* Iterate n times to reach a(n) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next values of m, d, a_cur */
        unsigned long long m_next = d * a_cur - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        /* Update state */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return (seq_t)a_cur;
}
