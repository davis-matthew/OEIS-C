/*
 * A086885  Lower triangular matrix, read by rows: T(i,j) = number of ways i
 *          seats can be occupied by any number k (0<=k<=j<=i) of persons.
 * https://oeis.org/A086885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j) where n = i*(i-1)/2 + j, 1 ≤ j ≤ i T(i,1) = i+1
 *            T(i,j) = T(i,j-1) + i * T(i-1,j-1) for j > 1 T(i,j) = T(j,i)
 *            (symmetry)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 76, u64 n <= 189,
 *            u128 n <= 527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient, works for all idx_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t d = 1; d <= k; ++d) {
        res = res * (n - k + d);
        res = res / d;          /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row i and column j of the lower triangular matrix
       such that n = i*(i-1)/2 + j, with 1 ≤ j ≤ i. */
    idx_t i = 1;
    while (((unsigned __int128)(i + 1) * i) / 2 < n) ++i;

    /* base = i*(i-1)/2 */
    idx_t base = (unsigned __int128)i * (i - 1) / 2;
    idx_t j = n - base;          /* 1 ≤ j ≤ i */

    if (j > i) {                  /* use symmetry T(i,j)=T(j,i) */
        idx_t tmp = i; i = j; j = tmp;
    }

    /* Compute T(i, j) using the summation formula
       T(i,j) = Σ_{k=0..j} k! * C(i,k) * C(j,k) */
    if (j == 1) return (seq_t)(i + 1);

    seq_t res = (seq_t)0;
    seq_t fac = (seq_t)1;        /* k! */
    for (idx_t k = 0; k <= j; ++k) {
        seq_t c1 = binom(i, k);
        seq_t c2 = binom(j, k);
        res += fac * c1 * c2;
        fac *= (k + 1);          /* next factorial */
    }
    return res;
}
