/*
 * A160520  a(n) is the number of distinct values in the periodic part of the
 *          continued fraction of the square root of n-th nonsquare positive
 *          integer.
 * https://oeis.org/A160520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of distinct values in the periodic part of the continued
   fraction of sqrt of the nth nonsquare positive integer. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the nth nonsquare positive integer */
    idx_t count = 0;
    idx_t m = 2;           /* start from 2, the first nonsquare */
    idx_t D = 0;
    while (1) {
        idx_t sq = int_sqrt(m);
        if (sq * sq != m) {            /* m is a nonsquare */
            count++;
            if (count == n) {
                D = m;
                break;
            }
        }
        m++;
    }

    /* Compute the continued fraction of sqrt(D) and its period */
    idx_t a0 = int_sqrt(D);
    idx_t m_val = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Store distinct values of the periodic part */
    size_t size_arr = 2 * (size_t)a0 + 1;   /* values are < 2*a0 + 1 */
    bool *seen = (bool *)malloc(size_arr);
    if (!seen) return 0;                   /* allocation failed; unlikely */
    for (size_t i = 0; i < size_arr; i++) seen[i] = false;

    /* Generate the period until a == 2*a0 */
    while (1) {
        idx_t m_next = d * a - m_val;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Store the value in the seen table */
        if (a_next < size_arr) seen[(size_t)a_next] = true;

        m_val = m_next;
        d = d_next;
        a = a_next;

        if (a == 2 * a0) break;   /* period complete */
    }

    /* Count distinct values */
    seq_t distinct = 0;
    for (size_t i = 1; i < size_arr; i++)
        if (seen[i]) distinct++;

    free(seen);
    return distinct;
}
