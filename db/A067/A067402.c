/*
 * A067402  Triangle with columns built from certain power sequences.
 * https://oeis.org/A067402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 80, u64 n <= 175,
 *            u128 n <= 441, big any n
 * Helpers:   int_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A067402 – Triangle with columns built from certain power sequences
 *
 *  The flattened sequence a(n) is obtained by enumerating the 
 *  triangle row‑by‑row (r,m) where r >= 0 and 0 <= m <= r:
 *      a(r,m) = 1                            if r == m
 *      a(r,m) = (2*m+1) * (m+1)^( 2*(r-m-1) )   if r > m >= 0
 *
 *  The function below finds the row r and column m that correspond
 *  to the linear index n, then evaluates the formula.  The computation
 *  uses only integer arithmetic and multiplies the power iteratively,
 *  so it works for any seq_t that supports +,-,*,/, and comparisons
 *  up to the size that fits in seq_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_pow_seq.h"

seq_t a(idx_t n)
{
    /* Find row r such that n < (r+1)(r+2)/2.
       Linear search is fine because r grows only as sqrt(n). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Offset within the row */
    idx_t prev_cum = r * (r + 1) / 2;  /* sum of lengths up to row r-1 */
    idx_t m = n - prev_cum;           /* column m in row r */

    if (r == m)
        return 1;                     /* diagonal elements */

    /* Compute (2*m+1) * (m+1)^(2*(r-m-1)) */
    seq_t term = (seq_t)(2 * (unsigned long long)m + 1);
    uint64_t exp = 2 * (r - m - 1);
    seq_t base = (seq_t)(m + 1);
    term *= int_pow_seq(base, exp);

    return term;
}
