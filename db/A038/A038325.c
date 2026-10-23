/*
 * A038325  Triangle read by rows whose (i,j)-th entry is
 *          binomial(i,j)*11^(i-j)*11^j.
 * https://oeis.org/A038325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 37, u64 n <= 125, u128 n <= 474,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038325: a(n) = binomial(i,j) * 11^i
   where i is the row index, j = n - i(i+1)/2. */

/* These utilities provide binomial coefficients and integer powers. */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include <math.h>
#include <stdint.h>

/* Return the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Determine row index i such that i(i+1)/2 <= n < (i+1)(i+2)/2. */
    idx_t i = (idx_t)((sqrt((long double)(8.0L * (long double)n + 1.0L)) - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one errors caused by floating‑point rounding. */
    idx_t start = i * (i + 1) / 2;
    while (start > n) { --i; start = i * (i + 1) / 2; }
    while (start + (i + 1) <= n) { ++i; start = i * (i + 1) / 2; }

    idx_t j = n - start;          /* column index in the row */

    seq_t bin = binomial(i, j);   /* C(i,j) */
    seq_t mult = pow_uint(11, i); /* 11^i */
    return bin * mult;
}
