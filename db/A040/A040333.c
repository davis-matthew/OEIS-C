/*
 * A040333  Continued fraction for sqrt(352).
 * https://oeis.org/A040333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) gives the n‑th partial quotient in the continued fraction
 *            expansion of sqrt(352). a(0) = floor(sqrt(352)), a(1) and onward
 *            repeat a periodic block ending with 2*a(0). The period length is
 *            8
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
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
    /* Compute floor(sqrt(352)) without floating point */
    const seq_t N = (seq_t)352;
    seq_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= N)
        ++a0;

    if (n == 0)
        return a0;

    /* Generate the period block [1,3,5,9,5,3,1,36] */
    seq_t block[32];
    size_t period = 0;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;

    while (1) {
        seq_t m_next = d * a_k - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;  /* floor division */
        block[period++] = a_next;
        if (a_next == 2 * a0)
            break;
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* Index into the repeated block */
    size_t idx = (size_t)((n - 1) % period);
    return block[idx];
}
