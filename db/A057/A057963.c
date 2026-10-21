/*
 * A057963  Triangle T(n,k) of number of minimal 2-covers of a labeled n-set
 *          that cover k points of that set uniquely (k=2,..,n).
 * https://oeis.org/A057963
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(n_set, k) * (2^(k-1) - 1) where the pair (n_set,
 *            k) is obtained from the linear index n by interpreting the OEIS
 *            triangle T(n_set,k) in row–major order, with offset 2 (i.e. the
 *            first term is a(2))
 * Range:     u8 n <= 19, u16 n <= 72, u32 n <= 243, u64 n <= 888,
 *            u128 n <= 3454, big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"       /* pow2(idx_t exp) */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2 (offset 2). */
    if (n < 2) return (seq_t)0;   /* undefined, return 0 for safety */

    /* Compute the 0‑based position of the index in the flattened triangle. */
    idx_t pos = n - 2;            /* pos >= 0 */

    /* Find the smallest integer s such that s*(s+1)/2 > pos.
       This s is one less than the row number n_set. */
    double d = sqrt((double)(8 * pos + 1));
    idx_t s = (idx_t)((d - 1.0) / 2.0);   /* floor value */
    if (s * (s + 1) / 2 <= pos) s++;     /* adjust if equality occurs */

    /* Now row number (size of the set) */
    idx_t n_set = s + 1;

    /* Cumulative number of entries up to the previous row */
    idx_t prevC = (n_set - 2) * (n_set - 1) / 2;

    /* Offset within the current row (0‑based) */
    idx_t offset = pos - prevC;

    /* Column index k (k >= 2) */
    idx_t k = offset + 2;

    /* Compute the triangle entry T(n_set, k) =
       binomial(n_set, k) * (2^(k-1) - 1) */
    seq_t bin = binomial(n_set, k);
    seq_t powv = pow2(k - 1);         /* 2^(k-1) */
    seq_t result = seq_mul(bin, powv - (seq_t)1);

    return result;
}
