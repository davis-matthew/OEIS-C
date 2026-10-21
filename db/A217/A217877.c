/*
 * A217877  Triangle read by rows: minimum inversion terminator in rooted
 *          labeled trees.
 * https://oeis.org/A217877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the concatenation of the rows of the triangle T defined
 *            by T(n,0) = (n‑1)!, T(n,k) = k! * (n‑k‑1) * n^(n‑k‑2) for 1 ≤ k
 *            ≤ n‑2. The sequence starts at a(2) = T(2,0). This routine maps
 *            the index n (≥2) to the corresponding (row,column) pair and
 *            evaluates T
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 57, u64 n <= 138,
 *            u128 n <= 380, big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

/* Return the n‑th term of A217877 (n ≥ 2).  The return type seq_t and
   the index type idx_t are defined by the build system. */
seq_t a(idx_t n) {
    /* The first valid index is 2.  Behaviour for n < 2 is undefined. */
    idx_t idx = n - 2;                /* zeroth‑based position in the flattened triangle */

    /* Find the row `i` such that the element lies in this row.  The
       row i has length (i‑1) (indices i = 2,3,…)                */
    idx_t i = 2;                      /* current row number */
    idx_t sum = 0;                    /* number of elements preceding this row */

    while (sum + (i - 1) <= idx) {
        sum += (i - 1);
        ++i;
    }

    /* Column index within the row (0 … i‑2). */
    idx_t j = idx - sum;

    if (j == 0) {
        /* T(i,0) = (i-1)! */
        return factorial(i - 1);
    } else {
        /* T(i,j) = j! * (i-j-1) * i^(i-j-2)   for 1 ≤ j ≤ i-2  */
        seq_t fact_j   = factorial(j);
        seq_t pow_val  = big_pow((seq_t)i, (seq_t)(i - j - 2));
        seq_t res      = fact_j * (seq_t)(i - j - 1);
        res            = res * pow_val;
        return res;
    }
}
