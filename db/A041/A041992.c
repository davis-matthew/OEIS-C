/*
 * A041992  Numerators of continued fraction convergents to sqrt(519).
 * https://oeis.org/A041992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   numerators of convergents to sqrt(519) via continued fraction
 * Range:     u8 none, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t used by idx_t */

seq_t a(idx_t n)
{
    static seq_t *num = NULL;
    static size_t len = 0, cap = 0;
    static bool initialized = false;

    /* state for continued‑fraction generation */
    static seq_t m_state = 0, d_state = 1, a_state;
    static seq_t a0 = 0;
    static const seq_t D = (seq_t)519;

    /* state for convergent recurrence */
    static seq_t h_im2 = 0, h_im1 = 1;   /* h[-2], h[-1] */
    static seq_t k_im2 = 1, k_im1 = 0;   /* k[-2], k[-1] */

    /* initialization */
    if (!initialized) {
        /* compute a0 = floor(sqrt(D)) by simple integer loop */
        seq_t i = 0;
        while ((i + 1) * (i + 1) <= D)
            i++;
        a0 = i;
        a_state = a0;           /* first partial quotient a[0] */

        cap = 8;
        num = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 0;
        initialized = true;
    }

    /* extend cache up to requested index */
    while (len <= n) {
        /* compute next numerator and denominator */
        seq_t h = a_state * h_im1 + h_im2;
        seq_t k = a_state * k_im1 + k_im2;

        /* store numerator */
        if (len >= cap) {
            cap *= 2;
            num = (seq_t *)realloc(num, cap * sizeof(seq_t));
        }
        num[len++] = h;

        /* update recurrence state */
        h_im2 = h_im1;
        h_im1 = h;
        k_im2 = k_im1;
        k_im1 = k;

        /* prepare next partial quotient a_state for the following index */
        seq_t m_next = d_state * a_state - m_state;
        seq_t d_next = (D - m_next * m_next) / d_state;
        seq_t a_next = (a0 + m_next) / d_next;

        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    return num[(size_t)n];
}
