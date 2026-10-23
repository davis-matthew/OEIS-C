/*
 * A042280  Numerators of continued fraction convergents to sqrt(666).
 * https://oeis.org/A042280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 17, u64 n <= 37, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(666) */
/* Computes P_n where P_n/Q_n are convergents to sqrt(666) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static storage for computed terms and continued‑fraction state */
    static seq_t *num = NULL;           /* numerators array */
    static idx_t num_cap = 0;           /* allocated capacity */
    static idx_t num_len = 0;           /* number of terms stored */

    static seq_t m = 0, d = 1;          /* Pell state variables */
    static seq_t a_coeff = 25;          /* current continued‑fraction term a_k */
    static seq_t Pprev2 = 0, Pprev1 = 1;/* previous two numerators */

    /* Allocate / grow the numerators array on the first call or when needed */
    if (num_cap == 0) {
        num_cap = 16;
        num = (seq_t *)malloc(num_cap * sizeof(seq_t));
    }
    while (num_len <= n) {
        /* if array full, grow it */
        if (num_len == num_cap) {
            idx_t new_cap = num_cap << 1;
            seq_t *tmp = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed, abort */
                exit(1);
            }
            num = tmp;
            num_cap = new_cap;
        }

        /* Compute next numerator using recurrence P_n = a_n * P_{n-1} + P_{n-2} */
        seq_t P = a_coeff * Pprev1 + Pprev2;
        num[num_len++] = P;            /* store numerator */

        /* Prepare next continued‑fraction term a_{k+1} */
        seq_t m_next = d * a_coeff - m;
        seq_t d_next = (666 - m_next * m_next) / d;
        seq_t a_next = (25 + m_next) / d_next;

        /* Shift recurrence state for next iteration */
        Pprev2 = Pprev1;
        Pprev1 = P;
        m = m_next;
        d = d_next;
        a_coeff = a_next;
    }

    return num[n];
}
