/*
 * A179753  Integers (2k)-1..0 followed by integers (2k)+1..0 and so on.
 * https://oeis.org/A179753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m·(m+1) – n where m is the unique integer with m·(m+1) ≥
 *            n > (m−1)·m
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A179753.
   a(1) = 1, a(2) = 0, a(3) = 3, a(4) = 2, a(5) = 1, a(6) = 0, … */

#include <math.h>  /* sqrtl */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 to avoid UB */
        return (seq_t)0;

    /* Find m such that m*(m+1) >= n.
       Use the quadratic solution m = floor((sqrt(1+4*n)-1)/2) and
       correct if necessary. */
    long double sq = sqrtl((long double)(1ULL + 4ULL * (unsigned long long)n));
    idx_t m = (idx_t)((sq - 1.0L) / 2.0L);   /* floor */

    /* Correct m to satisfy the inequality. */
    if ((idx_t)m * ((idx_t)m + 1ULL) < n)
        ++m;

    /* Compute a(n) = m*(m+1) – n.  All arithmetic done in seq_t. */
    seq_t total = (seq_t)m * ((seq_t)m + (seq_t)1);
    return total - (seq_t)n;
}
