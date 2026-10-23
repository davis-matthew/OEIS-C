/*
 * A038286  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*8^j.
 * https://oeis.org/A038286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle entries: T(i,j) = binomial(i,j) * 8^i.
   The sequence a(n) lists the entries in row‑major order (all of row 0,
   then all of row 1, etc.).  For a given n we find the row i and
   column j:
        i = floor((sqrt(8*n+1)-1)/2)
        j = n - i*(i+1)/2
   and return T(i,j). */
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row index i.  We use the formula
       i = floor((sqrt(8*n+1)-1)/2).  */
    long double d = (long double)8 * (long double)n + 1.0L;
    idx_t sqrt_val = (idx_t)floor(sqrt(d));   /* floor(sqrt(8*n+1)) */
    idx_t i = (sqrt_val - 1) / 2;

    /* Compute column j within that row. */
    idx_t start_of_row = i * (i + 1) / 2;     /* first index in row i */
    idx_t j = n - start_of_row;

    /* Compute the term binomial(i,j) * 8^i. */
    seq_t bin = binomial(i, j);
    seq_t pow = pow_uint(8, i);
    return bin * pow;
}
