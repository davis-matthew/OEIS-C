/*
 * A228154  T(n,k) is the number of s in {1,...,n}^n having longest contiguous
 *          subsequence with the same value of length k; triangle T(n,k),
 *          n>=1, 1<=k<=n, read by rows.
 * https://oeis.org/A228154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c), where r is the smallest integer with * r(r+1)/2 ≥
 *            n, c = n − r(r−1)/2, and T(r,c) is the number of * length‑r
 *            strings over an r‑letter alphabet whose longest run of * equal
 *            symbols has length exactly c. T(r,c) = Count(r,c) –
 *            Count(r,c−1), * where Count(r,k) counts strings with long...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 46, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   count_leq_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include "../utils/count_leq_run.h"

/*  The triangle entry T(n,k).  */
static seq_t
T(idx_t n, idx_t k)
{
    if (k == 0 || k > n)
        return 0;
    if (k == n)
        return (seq_t)n;                 /* all symbols equal */

    seq_t a = count_leq_run(n, k);
    seq_t b = count_leq_run(n, k - 1);
    return a - b;
}

/*  The demanded sequence function.  */
seq_t
a(idx_t n)
{
    /*  Find the row r such that r(r+1)/2 ≥ n > (r−1)r/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)  /*  r+1 iterations  */
        ++r;

    /*  Now r satisfies r(r+1)/2 < n ≤ (r+1)(r+2)/2  */
    r++;                                 /*  shift to 1‑based row */

    /*  Column index c  */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t c = n - prev_total;            /*  1 ≤ c ≤ r  */

    return T(r, c);
}
