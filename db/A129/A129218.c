/*
 * A129218  Permutations with exactly 10 fixed points.
 * https://oeis.org/A129218
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,10) * D(n-10) where D(m) = derangement number,
 *            D(0)=1, D(1)=0, D(m) = (m-1)*(D(m-1)+D(m-2)) for m≥2
 *            (permutations of n elements with exactly 10 fixed points)
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 18, u64 n <= 25, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 197 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Compute derangement D(m) iteratively, avoiding dynamic allocation.
   Works for any seq_t implementation that supports +, -, *, /, %
   and for any size of m. */
static seq_t derangement_upto(idx_t m)
{
    if (m == 0)
        return (seq_t)1;
    if (m == 1)
        return (seq_t)0;

    seq_t d_prev2 = (seq_t)1;   /* D(0) */
    seq_t d_prev1 = (seq_t)0;   /* D(1) */
    seq_t d_cur   = d_prev1;    /* placeholder */

    for (idx_t i = 2; i <= m; ++i) {
        seq_t sum   = d_prev1 + d_prev2;              /* D(i-1)+D(i-2) */
        d_cur = (seq_t)(i - 1) * sum;                 /* (i-1)*(...)  */
        d_prev2 = d_prev1;
        d_prev1 = d_cur;
    }
    return d_cur;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 10)                     /* values below offset are undefined */
        return (seq_t)0;

    idx_t m = n - 10;               /* number of non‑fixed positions */
    seq_t der = derangement_upto(m);/* D(n-10) */
    seq_t bin = binomial(n, (idx_t)10);/* C(n,10) */
    return bin * der;
}
