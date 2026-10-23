/*
 * A155169  Triangle sequence:t(n,m)=(2 n - Binomial[n, m] + m! + (-m + n)!).
 * https://oeis.org/A155169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   a(n) returns the n‑th entry of the triangle * t(n,m) = 2*n -
 *            binomial(n,m) + m! + (n-m)! * The triangle is listed row by row
 *            (row 0: 1, row 1: 3,3, …). * Indexing starts at 0
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine row r and column m for index n */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1.0) - 1.0) / 2.0);
    /* correct possible off‑by‑one due to floating point */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;

    /* compute the triangle entry */
    seq_t r_seq = (seq_t) r;
    seq_t bin = binomial(r, m);
    seq_t fact_m   = factorial(m);
    seq_t fact_nm  = factorial(r - m);

    seq_t result = 2 * r_seq - bin + fact_m + fact_nm;
    return result;
}
