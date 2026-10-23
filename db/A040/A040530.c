/*
 * A040530  Continued fraction for sqrt(554).
 * https://oeis.org/A040530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(554).  
   a(0) = 23; for n >= 1 the sequence repeats with period 11:  
   1, 1, 6, 4, 1, 1, 4, 6, 1, 1, 46.  */
/* The algorithm computes the period on first use and then
   indexes it modulo 11.  Each call works for any n >= 0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t periodic[20];   /* holds the periodic block up to 46 */
    static size_t period_len = 0;

    /* Compute the periodic part only once. */
    if (period_len == 0) {
        const idx_t D   = (idx_t)554;
        const idx_t a0  = (idx_t)23;   /* a(0) */
        idx_t m = 0;                  /* m0 */
        idx_t d = 1;                  /* d0 */
        idx_t a_term = a0;            /* a0 as start for computation */
        size_t i = 0;
        idx_t a_next;

        /* Generate the continued‑fraction terms until a term
           equals 2*a0.  That term is included and marks the end
           of the period. */
        do {
            idx_t m_next = d * a_term - m;
            idx_t d_next = (D - m_next * m_next) / d;
            a_next = (a0 + m_next) / d_next;

            periodic[i++] = (seq_t)a_next;

            m = m_next;
            d = d_next;
            a_term = a_next;
        } while (a_next != (idx_t)(2 * a0));

        period_len = i;   /* should be 11 for sqrt(554) */
    }

    if (n == (idx_t)0)
        return (seq_t)23;   /* a(0) */

    /* Locate the position within the periodic block. */
    size_t idx = (size_t)((n - (idx_t)1) % (idx_t)period_len);
    return periodic[idx];
}
