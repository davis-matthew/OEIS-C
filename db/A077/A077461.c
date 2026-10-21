/*
 * A077461  Duplicate of A077444.
 * https://oeis.org/A077461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 2, a(2) = 14, a(n) = 6*a(n‑1) – a(n‑2) for n ≥ 3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but never called */

    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)14;

    /* iterate using only the two most recent values */
    seq_t a_nm2 = (seq_t)2;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)14;  /* a(n-1) */
    seq_t an = a_nm1;         /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        an = ((seq_t)6) * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
