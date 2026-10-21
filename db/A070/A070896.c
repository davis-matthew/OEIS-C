/*
 * A070896  Determinant of the Cayley addition table of Z_{n}.
 * https://oeis.org/A070896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^floor(n/2) * (1/2) * (n-1) * n^(n-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial offsets explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;

    seq_t base = (seq_t)n;
    seq_t result;

    /* The product (n-1)*n^(n-1) is always even.
       Separate the factor 2 to avoid truncation. */
    if (n & 1u) {                 /* n is odd: (n-1) is even */
        idx_t half = (n - 1) >> 1;        /* (n-1)/2 as idx_t */
        seq_t power = seq_pow(base, n - 1);  /* n^(n-1) */
        result = (seq_t)half * power;      /* (n-1)/2 * n^(n-1) */
    } else {                      /* n is even: n is even, */
        seq_t power = seq_pow(base, n - 2);  /* n^(n-2)  */
        seq_t term   = (seq_t)(n - 1) * (seq_t)(n >> 1); /* (n-1)*(n/2) */
        result = term * power;                /* (n-1)*(n/2)*n^(n-2) */
    }

    /* Apply the sign: negative when floor(n/2) is odd */
    if ((n >> 1) & 1u)
        result = -result;

    return result;
}
