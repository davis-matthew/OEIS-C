/*
 * A154557  Production array of A122848, read by row.
 * https://oeis.org/A154557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 44, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Production array of A122848 (read by row).
   For row r (r ≥ 0) the values are indexed by c = 0 … r+1:
     a(r·(r+3)/2 + 0)   = 0
     a(r·(r+3)/2 + r+1) = 1
   For 1 ≤ c ≤ r:
     a(r·(r+3)/2 + c) = (-1)^(r-c) · binom(r, c-1) · (2r – 2c – 1)!!   */
#include <stdint.h>
#include "../utils/binomial.h"

/* odd double factorial for an integer odd (odd = 1,3,5,…) ;
   (–1)!! is defined as 1;  for odd ≤ 1 the loop is skipped. */
static inline seq_t odd_df(long long odd)
{
    seq_t result = 1;
    for (long long t = odd; t > 1; t -= 2)
        result *= (seq_t)t;
    return result;
}

/* linear index of the first element of row r */
static inline idx_t start_of_row(idx_t r)
{
    return (r * r + 3 * r) >> 1;        /* (r² + 3r)/2 */
}

seq_t a(idx_t n)
{
    /* locate the row r containing index n */
    idx_t r = 0;
    while (start_of_row(r + 1) <= n)
        ++r;

    idx_t c = n - start_of_row(r);   /* column within the row, 0‑based */

    /* first and last elements of every row */
    if (c == 0)
        return 0;
    if (c == r + 1)
        return 1;

    /* interior elements */
    seq_t bin = binomial(r, c - 1);          /* binomial coefficient */
    long long odd = 2 * (long long)r - 2 * (long long)c - 1; /* (2r‑2c‑1) */
    seq_t val = bin * odd_df(odd);

    /* apply the alternating sign */
    if ((r - c) & 1)
        val = -val;

    return val;
}
