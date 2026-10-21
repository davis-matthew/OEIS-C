/*
 * A090663  Second term in continued fraction for the n-th root of n.
 * https://oeis.org/A090663
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A090663
 *  second term in the continued fraction for the n‑th root of n
 *
 *  a(n) = floor( 1 / ( n^(1/n) - 1 ) )
 *        = max{ k ≥ 1 | n ≤ (1 + 1/k)^n }
 *
 *  For each n ≥ 2 the sequence value is the largest k such that
 *  n ≤ (1 + 1/k)^n.
 *
 *  We compute it by a simple binary search on k, using logarithms to
 *  avoid overflow and to keep the computation accurate for all n up
 *  to at least 10 000 (and beyond).  The comparison
 *          ln n  ≤  n × ln(1+1/k)
 *  is performed in double precision; an epsilon is added to guard
 *  against rounding errors.
 */

#include <math.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2; value for n<2 is undefined */

    /* Work in double for the comparison */
    const double ln_n = log((double) n);

    /* Rough upper bound for k : n / log n  +  a safety margin  */
    size_t high;
    if (n > 2) {
        double est = (double)n / log((double)n);
        high = (size_t)est + 50;    /* larger than n/log n for all n ≥ 3 */
        if (high < 50) high = 50;   /* small n */
        if (high > n) high = (size_t)n;
    } else {
        high = 50;                  /* for n == 2 */
    }

    size_t low = 1, best = 1;
    const double eps = 1e-12;     /* safety margin for floating errors */

    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        double right = (double) n * log1p(1.0 / (double)mid);
        if (ln_n <= right + eps) {      /* condition n ≤ (1+1/mid)^n holds */
            best = mid;
            low = mid + 1;              /* try larger k */
        } else {
            if (mid == 0) break;        /* avoid infinite loop */
            high = mid - 1;             /* try smaller k */
        }
    }

    return (seq_t)best;
}
