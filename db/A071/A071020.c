/*
 * A071020  Square array T(i,j) = Bernoulli(2i)*Bernoulli(2j) read by
 *          antidiagonals: numerators.
 * https://oeis.org/A071020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, tabl
 * Formula:   product of even‑index Bernoulli numbers read by antidiagonals.
 *            a(n) is the numerator of B_{2i} * B_{2j} where (i,j) is the
 *            coordinates obtained by reading the infinite square array
 *            T(i,j)=B_{2i}*B_{2j} along antidiagonals, starting at (0,0) for
 *            n=0
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 65, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   alloc_seq_array, antidiagonal_coordinates, binom,
 *            free_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/binom.h"
#include "../utils/antidiagonal_coordinates.h"

/* Cached Bernoulli numbers */
static seq_t *B_num = NULL;   /* numerator of B_k          */
static seq_t *B_den = NULL;   /* denominator of B_k (positive) */
static idx_t  B_size = 0;     /* number of entries cached (k=0..B_size-1) */

/* Reduce a rational number (num/den) and return the new numerator and denominator */
static void reduce_rational(seq_t *num, seq_t *den)
{
    seq_t g = gcd((*num >= 0 ? *num : -*num), *den);
    if (g != 0) {
        *num /= g;
        *den /= g;
    }
}

/* Compute all Bernoulli numbers B_k (k = 0 … need) using the standard recurrence.
   The values are stored in the cached arrays B_num and B_den.  On return
   B_size == need+1. */
static void ensure_bernoulli(idx_t need)
{
    /* Allocate fresh arrays – sized at most a few hundred entries.
       Free any previous storage. */
    if (B_num) free_seq_array(B_num);
    if (B_den) free_seq_array(B_den);
    B_num = alloc_seq_array(need + 1);
    B_den = alloc_seq_array(need + 1);

    /* B_0 = 1 */
    B_num[0] = 1;
    B_den[0] = 1;

    for (idx_t m = 1; m <= need; ++m)
    {
        /* sum = Σ_{k=0}^{m-1} C(m+1,k) * B_k */
        seq_t sum_num = 0;
        seq_t sum_den = 1;

        for (idx_t k = 0; k < m; ++k)
        {
            seq_t bin   = binom(m + 1, k);          /* C(m+1,k) */
            seq_t t_num = bin * B_num[k];           /* numerator of term */
            seq_t t_den = B_den[k];                /* denominator of term */

            /* sum += t_num / t_den */
            seq_t g  = gcd(sum_den, t_den);
            seq_t lcm_den = (sum_den / g) * t_den;
            sum_num = sum_num * (lcm_den / sum_den) + t_num * (lcm_den / t_den);
            sum_den = lcm_den;
            reduce_rational(&sum_num, &sum_den);
        }

        /* B_m = -sum / (m+1) */
        seq_t bm_num = -sum_num;
        seq_t bm_den = sum_den * (seq_t)(m + 1);
        reduce_rational(&bm_num, &bm_den);

        B_num[m] = bm_num;
        B_den[m] = bm_den;
    }

    B_size = need + 1;
}

/* The required OEIS sequence function */
seq_t a(idx_t n)
{
    /* Find the (i,j) corresponding to index n */
    idx_t i, j;
    antidiagonal_coordinates(n, &i, &j);

    /* Ensure Bernoulli numbers up to needed indices are available */
    idx_t need = 2 * (i > j ? i : j);   /* maximal even index needed */
    if (B_size <= need)                /* compute only if missing */
        ensure_bernoulli(need);

    idx_t idx_i = 2 * i;
    idx_t idx_j = 2 * j;

    /* Compute the numerator of the product B_{2i} * B_{2j} */
    seq_t num = B_num[idx_i] * B_num[idx_j];
    seq_t den = B_den[idx_i] * B_den[idx_j];

    /* Reduce the fraction and return the numerator */
    reduce_rational(&num, &den);   /* numerator now in simplest form */
    return num;
}
