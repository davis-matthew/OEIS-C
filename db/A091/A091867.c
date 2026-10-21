/*
 * A091867  Triangle read by rows: T(n,k) = number of Dyck paths of semilength
 *          n having k peaks at odd height.
 * https://oeis.org/A091867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r,c are determined by n = r*(r+1)/2 + c (0 ≤
 *            c ≤ r); T(r,c) = r(r−c) * C(r,c), r(m) = A005043(m) (Riordan
 *            numbers)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 235, u64 n <= 748,
 *            u128 n <= 2570, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* cache for Riordan numbers r[0..] */
static seq_t *riordan      = NULL;
static idx_t   riordan_len = 0;   /* number of computed elements */
static idx_t   riordan_cap = 0;   /* current allocation size */

/* ensure riordan array has values up to index n (inclusive) */
static void ensure_riordan_upto(idx_t n)
{
    if (riordan_len > n) return;

    idx_t new_cap = riordan_cap ? riordan_cap : 1;
    if (new_cap <= n) new_cap = n + 1;
    while (new_cap <= n) new_cap <<= 1;

    /* reallocate storage; cast to correct type for C/C++ */
    seq_t *tmp = (seq_t *)realloc(riordan, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* allocation failed */

    riordan      = tmp;
    riordan_cap  = new_cap;

    /* compute new Riordan numbers */
    for (idx_t i = riordan_len; i <= n; ++i) {
        if (i == 0) {
            riordan[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += riordan[j] * binomial(i, j);
            seq_t cat = catalan(i);
            riordan[i] = cat - sum;
        }
    }
    riordan_len = n + 1;
}

/* public function giving the sequence value at position n */
seq_t a(idx_t n)
{
    /* find the row (r) and column (c) corresponding to n
       in the triangle read by rows */
    idx_t row = 0;
    idx_t rem = n;
    while (rem >= row + 1) {
        rem -= row + 1;
        ++row;
    }
    idx_t col = rem;                /* 0 ≤ col ≤ row */

    /* ensure we have Riordan numbers up to (row - col) */
    ensure_riordan_upto(row);

    /* T(row,col) = r(row-col) * C(row,col) */
    seq_t bin = binomial(row, col);
    seq_t res = riordan[row - col] * bin;
    return res;
}
