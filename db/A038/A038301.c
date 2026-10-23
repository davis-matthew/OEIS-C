/*
 * A038301  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*11^j.
 * https://oeis.org/A038301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i , j)*9^(i‑j)*11^j, where i and j are
 *            determined by: i = smallest non‑negative integer with
 *            (i+1)(i+2)/2 > n j = n – i(i+1)/2 (the offset within row i)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 38, u64 n <= 140, u128 n <= 507,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of A038301 (triangular flattening of the matrix
   with entries binomial(i,j)*9^(i-j)*11^j). */
seq_t a(idx_t n)
{
    /* Find the row i that contains the n-th element. */
    idx_t i = 0;
    while ((__int128)(i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* Compute j, the column offset within that row. */
    idx_t triangular_i = (i * (i + 1)) / 2;   /* i*(i+1)/2 fits in idx_t */
    idx_t j = n - triangular_i;               /* 0 <= j <= i */

    /* Compute binomial(i , j) multiplicatively. */
    seq_t bin = 1;
    for (idx_t k = 1; k <= j; ++k) {
        bin *= (seq_t)(i + 1 - k);
        bin /= (seq_t)k;
    }

    /* Now multiply by 9^(i-j) and 11^j. */
    seq_t pow9 = 1;
    for (idx_t k = 0; k < i - j; ++k)
        pow9 *= (seq_t)9;

    seq_t pow11 = 1;
    for (idx_t k = 0; k < j; ++k)
        pow11 *= (seq_t)11;

    /* Final value. */
    seq_t val = bin * pow9 * pow11;
    return val;
}
