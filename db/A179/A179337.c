/*
 * A179337  Positive integers of the form (6*m^2 + 1)/11.
 * https://oeis.org/A179337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (66*n*(n-1) - 3*(2*n-1)*(-1)^n + 17)/4
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8607, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Not an official offset, safe fallback */

    seq_t n_seq = (seq_t)n;
    seq_t term1 = (seq_t)66 * n_seq * (n_seq - 1);   /* 66*n*(n-1) */
    seq_t k = (seq_t)(2 * n_seq - 1);               /* 2*n-1 */
    seq_t numerator = term1 + (seq_t)17;            /* initial term1 + 17 */

    if ((n % 2) == 0) {                      /* even n: subtract 3*(2*n-1) */
        numerator -= (seq_t)3 * k;
    } else {                                 /* odd n: add   3*(2*n-1) */
        numerator += (seq_t)3 * k;
    }

    return numerator / (seq_t)4;             /* divide by 4 */
}
