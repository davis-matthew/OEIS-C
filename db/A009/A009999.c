/*
 * A009999  Triangle in which j-th entry in i-th row is (i+1-j)^j, 0<=j<=i.
 * https://oeis.org/A009999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the n‑th element of the triangle defined by T(i,j) =
 *            (i+1-j)^j (0 ≤ j ≤ i) and flattened row‑major. Steps to compute
 *            a(n): 1. Find row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. 2.
 *            j = n - i*(i+1)/2. 3. a(n) = (i+1-j)^j
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 181, u64 n <= 452,
 *            u128 n <= 1159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Computes the A009999 sequence value for index n. */
seq_t a(idx_t n)
{
    /* Find row index i.  Initial approximation from the quadratic formula. */
    idx_t i = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);

    /* Correct i to satisfy i*(i+1)/2 ≤ n. */
    while (((__int128)(i + 1) * (i + 2)) / 2 <= n) i++;
    while (((__int128)i * (i + 1)) / 2 > n)   i--;

    /* Compute j = n - start_of_row(i). */
    idx_t row_start = (idx_t)(((__int128)i * (i + 1)) / 2);
    idx_t j = n - row_start;

    /* Base of the power. */
    seq_t base = (seq_t)((i + 1) - j);

    /* Compute base^j by repeated multiplication (j ≤ i ≤ 141 for n ≤ 10000). */
    seq_t result = 1;
    for (idx_t k = 0; k < j; ++k)
        result *= base;

    return result;
}
