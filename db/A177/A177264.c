/*
 * A177264  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k as the first entry in the last block
 *          (1<=k<=n).
 * https://oeis.org/A177264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = the linearisation of triangle T(n,k) where * T(n,1) =
 *            0! + 1! + … + (n-1)! (col==1) * T(n,k) = (n-1)! – (k-2)! (2 ≤ k
 *            ≤ n) * * For a sequence index n (1‑based) we first determine the
 *            row r such that * r*(r-1)/2 < n ≤ r*(r+1)/2 * Let c = n –
 *            r*(r-1)/2 (column, 1 ≤ c ≤ r). Then * a(n) = T(...
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* Find the row r that contains the n-th term.  */
    idx_t r = 1;
    idx_t prefix = 0;        /* sum of lengths of rows before r */
    while (prefix + r < n) { /* keep adding next row until n is inside */
        prefix += r;
        r++;
    }
    idx_t c = n - prefix;    /* column number within row r (1‑based) */

    /* Compute T(r,c).  */
    if (c == 1) {
        /* T(r,1) = 0! + 1! + … + (r-1)!  */
        seq_t sum = 1;      /* 0! */
        seq_t f = 1;        /* current factorial value i!  */
        for (idx_t i = 1; i < r; ++i) {
            f *= (seq_t)i;   /* i! = (i-1)! * i */
            sum += f;
        }
        return sum;
    } else {
        /* T(r,c) = (r-1)! – (c-2)!  */
        seq_t fact_r_minus_1 = factorial(r - 1);
        seq_t fact_c_minus_2 = factorial(c - 2);
        return fact_r_minus_1 - fact_c_minus_2;
    }
}
