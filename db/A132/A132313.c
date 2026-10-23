/*
 * A132313  Integer pair values {n,m} near the line: m=-Log[2]/Log[3] +
 *          (Log[4]/Log[3])*n Based on musical scales of the Pythagorean
 *          triangle type{3,4,5} where 4^n/3^m is near 2. The line gives
 *          values of 2 exactly for real numbers.
 * https://oeis.org/A132313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132313 (OEIS). 
   Terms are generated from the recurrence
   n_1 = 10, m_1 = 12.
   For i >= 2:
     if i is even: n_i = n_{i-1} + 23, m_i = m_{i-1} + 29;
     if i is odd : n_i = n_{i-1} + 19, m_i = m_{i-1} + 24;
   The sequence a(n) alternates the n_i and m_i values:
     a(2k-1) = n_k,  a(2k) = m_k   (k >= 1).
   This simple recurrence reproduces the listed terms. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the pair index k = (n + 1) / 2. */
    idx_t k = (n + 1) / 2;
    /* Number of complete 2‑step cycles after the first pair. */
    idx_t cycles = (k - 1) / 2;
    idx_t rem    = (k - 1) % 2;

    /* Base values for the first pair. */
    seq_t n_k = 10;
    seq_t m_k = 12;

    /* Add contributions from complete cycles (23+19 = 42, 29+24 = 53). */
    n_k += (seq_t)42 * cycles;
    m_k += (seq_t)53 * cycles;

    /* Add the partial increment if there is a remainder. */
    if (rem == 1) {
        n_k += 23;
        m_k += 29;
    }

    /* Return the appropriate component of the pair. */
    if (n & 1)  /* odd index -> n_k */
        return n_k;
    else        /* even index -> m_k */
        return m_k;
}
