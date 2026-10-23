/*
 * A040965  Continued fraction for sqrt(997).
 * https://oeis.org/A040965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients for sqrt(997). This function
 *            computes the n‑th term a(n) of the continued fraction of
 *            sqrt(997). It memoises the coefficients after the first call, so
 *            the cost is O(1) for subsequent queries. The sequence is
 *            periodic after the first term. a(0) = ⌊√997⌋, and for n ≥...
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of the continued fraction coefficients */
    static seq_t *coeffs = NULL;          /* array of coefficients   */
    static size_t  coeff_len = 0;          /* number of coefficients  */
    static size_t  coeff_cap = 0;          /* current capacity        */
    static bool    initialized = false;   /* cache initialised?      */
    static seq_t   a0;                    /* floor(sqrt(997))        */
    static idx_t   periodlen = 0;          /* length of period (excluding a0) */

    /* initialise the cache on first call */
    if (!initialized) {
        /* compute a0 = ⌊√997⌋ */
        a0 = floor_sqrt((seq_t)997);

        /* push the first term a0 */
        append_candidate(&coeffs, &coeff_len, &coeff_cap, a0);

        /* continued fraction loop for sqrt(997) */
        seq_t m   = (seq_t)0;
        seq_t d   = (seq_t)1;
        seq_t a_i = a0;
        while (1) {
            seq_t m_next = d * a_i - m;
            seq_t d_next = ((seq_t)997 - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;   /* floor division */

            append_candidate(&coeffs, &coeff_len, &coeff_cap, a_next);

            /* termination when the period completes: a_next == 2*a0 */
            if (a_next == (a0 << 1))
                break;

            /* prepare for next iteration */
            m   = m_next;
            d   = d_next;
            a_i = a_next;
        }

        periodlen   = coeff_len - 1;     /* exclude the a0 term */
        initialized = true;
    }

    /* retrieve the requested term */
    if (n == 0)
        return coeffs[0];

    if (n <= periodlen)
        return coeffs[(size_t)n];

    /* wrap around the periodic part */
    idx_t pos = ((n - 1) % periodlen) + 1;
    return coeffs[(size_t)pos];
}
