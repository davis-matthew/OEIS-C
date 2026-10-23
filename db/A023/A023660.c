/*
 * A023660  Convolution of odd numbers and A023533.
 * https://oeis.org/A023660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k ≤ n, k is a tetrahedral number} ( 2*(n‑k) + 1 ) *
 *            Equivalently: a(n) = t(n)*(2n+1) – 2*s(n), * where t(n) is the
 *            number of tetrahedral numbers ≤ n * and s(n) is their sum
 * Range:     u8 n <= 39, u16 n <= 2063, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types defined by the build */
#include <stdint.h>
/* The types seq_t and idx_t are supplied externally; no redefinition. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* Count of tetrahedral numbers ≤ n and their sum. */
    idx_t  count = 0;
    seq_t  sum_t = 0;

    /* Generate tetrahedral numbers in order using the recurrence
     * T(1) = 1,  T(k) = T(k‑1) + triangular(k+1) where triangular(m)=m*(m+1)/2.
     */
    idx_t  k = 1;                /* current index in the tetrahedral sequence */
    seq_t  tetra = 1;            /* current tetrahedral number */

    while (tetra <= n)
    {
        ++count;
        sum_t += tetra;

        /* Advance to the next tetrahedral number.
         * Triangular number for m = k+1: m*(m+1)/2
         */
        idx_t m = k + 1;
        seq_t tri = (seq_t)m * (m + 1) / 2;
        tetra += tri;          /* next tetrahedral number */

        ++k;
    }

    /* Compute the final value: a(n) = count*(2n+1) – 2*sum_t */
    seq_t term = (seq_t)count * ((seq_t)n * 2 + 1) - (seq_t)2 * sum_t;
    return term;
}
