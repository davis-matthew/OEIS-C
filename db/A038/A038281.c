/*
 * A038281  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*3^j.
 * https://oeis.org/A038281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = sequence obtained by enumerating the triangle whose
 *            (i,j)-th entry equals binom(i,j)*8^(i-j)*3^j row by row,
 *            starting at i=0, j=0. The index n is converted to the
 *            appropriate (i,j) pair using the triangular-number mapping
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 56, u64 n <= 211, u128 n <= 748,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 998 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper routines are declared in the utils library that
   accompanies the build.  We include the minimal set of headers
   to obtain the prototypes. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute a(n).  idx_t is an unsigned 64‑bit type; seq_t is an
   unsigned integer type possibly arbitrary‐precision. */
seq_t a(idx_t n)
{
    /* Find row i such that the cumulative number of entries up to row i
       (inclusive) exceeds n.  The starting index of row i is
       S(i) = i*(i+1)/2.  */
    idx_t i = 0;
    while (true)
    {
        /* Start index of the next row i+1. */
        idx_t next = (i + 1) * (i + 2) / 2;
        if (next > n) break;
        ++i;
    }

    /* Column j within the found row. */
    idx_t j = n - i * (i + 1) / 2;

    /* Compute the binomial coefficient, then multiply by the power
       factors.  All arithmetic is performed in seq_t. */
    seq_t term = binomial(i, j);
    term = seq_mul(term, ipow_i32((seq_t)8, (int)(i - j)));
    term = seq_mul(term, ipow_i32((seq_t)3, (int)j));

    return term;
}
