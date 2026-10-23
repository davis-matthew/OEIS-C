/*
 * A177229  Triangle, read by rows, T(n, k) = -binomial(n,k) for 1 <= k <=
 *          n-1, otherwise T(n, k) = 4.
 * https://oeis.org/A177229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less, easy
 * Formula:   a(n) = 4 if k==0 or k==r else -binomial(r,k), * where r =
 *            floor((sqrt(8*n+1)-1)/2), * k = n - r*(r+1)/2
 * Range:     i8 n <= 7, i16 n <= 178, i32 n <= 609, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute binomial coefficient C(n,k) for 0 ≤ k ≤ n.
 * The result fits into seq_t. */
static seq_t binom_val(idx_t n, idx_t k)
{
    if (k > n)          return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n / 2)      k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2). */
    seq_t nseq      = (seq_t)n;
    seq_t disc      = nseq * (seq_t)8 + (seq_t)1;      /* 8*n + 1  */
    seq_t sqrt_disc = floor_sqrt(disc);                /* floor sqrt */
    seq_t rseq      = (sqrt_disc - (seq_t)1) / (seq_t)2;
    idx_t r         = (idx_t)rseq;                       /* row index */

    /* offset = r*(r+1)/2 : first index of the row */
    seq_t offset_seq = (seq_t)r * (seq_t)(r + 1) / (seq_t)2;
    idx_t offset     = (idx_t)offset_seq;

    /* position within the row */
    idx_t k = n - offset;   /* 0 ≤ k ≤ r */

    if (k == 0 || k == r)
        return (seq_t)4;
    else
        return -binom_val(r, k);
}
