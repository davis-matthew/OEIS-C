/*
 * A014775  Expansion of exp ( - x - (1/2)*x^2 - (1/6)*x^3).
 * https://oeis.org/A014775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = -1, a(2) = 0; For n >= 0, a(n+1) = -a(n) -
 *            n*a(n-1) - (n*(n-1)/2)*a(n-2)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A014775: Expansion of exp(-x - (1/2)*x^2 - (1/6)*x^3) */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)0;

    /* a(n-2), a(n-1), a(n) for the current k */
    seq_t an_minus2 = (seq_t)1;  /* a(0) */
    seq_t an_minus1 = (seq_t)-1; /* a(1) */
    seq_t an        = (seq_t)0;  /* a(2) */

    for (idx_t k = 2; k < n; ++k) {
        /* Compute a(k+1) */
        seq_t term1 = -an;
        seq_t term2 = -(seq_t)k * an_minus1;
        seq_t m1 = (seq_t)k * (seq_t)(k - 1) / 2;
        seq_t term3 = -m1 * an_minus2;
        seq_t an_next = term1 + term2 + term3;

        /* Shift for next iteration */
        an_minus2 = an_minus1;
        an_minus1 = an;
        an        = an_next;
    }
    return an;
}
