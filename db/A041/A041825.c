/*
 * A041825  Denominators of continued fraction convergents to sqrt(433).
 * https://oeis.org/A041825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* denom_cf433.c
 * Denominators of continued fraction convergents to sqrt(433).
 * The sequence is given by
 *   q_(-1) = 0,   q_0   = 1
 *   q_n   = a_n * q_{n-1} + q_{n-2}   (n ≥ 1)
 * where a_0 = 20 and for n ≥ 1
 *   a_n = [1,4,4,2,2,1,3,13,1,1,1,1,13,3,1,2,2,4,4,1,40]_{(n-1) mod 21}
 * The function returns the n'th term of the sequence (a(0) = 1).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *den = NULL;
    static idx_t sz = 0;   /* number of stored terms */

    /* coefficients of the continued fraction (period 21) */
    static const seq_t period[21] = {
        1, 4, 4, 2, 2, 1, 3, 13, 1, 1, 1, 1, 13, 3, 1, 2, 2, 4, 4, 1, 40
    };

    if (n >= sz) {
        /* grow the cache to hold up to n */
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(den, new_sz * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed: terminate */
        den = tmp;

        /* compute new terms */
        for (idx_t k = sz; k <= n; ++k) {
            if (k == 0) {
                den[k] = (seq_t)1;                 /* q0 = 1 */
            } else if (k == 1) {
                /* coefficient a1 = period[0] = 1 */
                den[k] = period[0] * den[k-1];     /* q1 = a1*q0 + q_-1 (q_-1 = 0) */
            } else {
                seq_t coeff = period[(k - 1) % 21];
                den[k] = coeff * den[k-1] + den[k-2];
            }
        }
        sz = new_sz;
    }

    return den[n];
}
