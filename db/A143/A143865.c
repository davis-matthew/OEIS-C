/*
 * A143865  Eigentriangle of A099375 (odd number subsequences decrescendo).
 * https://oeis.org/A143865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143865: Eigentriangle of A099375 (odd number subsequences decrescendo)
   For row i (i ≥ 1) the odd sequence is 2*(i-j)+1 for j = 1..i.
   It is term‑wise multiplied by A003946(j), where
        A003946(1) = A003946(2) = 1,
        A003946(j) = 4 * 3^(j-3)   for j ≥ 3.
   The triangle is read row by row, so the n‑th term a(n) is obtained as follows:

      1. Find the row i such that i*(i+1)/2 ≥ n.
      2. Let j = n - (i-1)*i/2   (the column in that row).
      3. a(n) = ( 2*(i-j) + 1 ) * A003946(j).
*/

#include <stdint.h>

/* compute 3^exp using repeated multiplication */
static seq_t power_of_three(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t k = 0; k < exp; ++k)
        res *= (seq_t)3;
    return res;
}

/* A003946(j) */
static seq_t a003946(idx_t j)
{
    if (j <= 2)
        return (seq_t)1;
    return (seq_t)4 * power_of_three(j - 3);
}

/* OEIS sequence A143865 */
seq_t a(idx_t n)
{
    /* find the row i (1‑based) containing the n‑th element */
    idx_t i = 1;
    while ((i * (i + 1ULL) / 2ULL) < n)
        ++i;

    /* column j in that row */
    idx_t prev = (i - 1) * i / 2ULL;
    idx_t j = n - prev;

    seq_t val_a_j = a003946(j);
    seq_t mult = (seq_t)(2ULL * (i - j) + 1ULL);
    return val_a_j * mult;
}
