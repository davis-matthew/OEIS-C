/*
 * A235333  a(n) is the smallest k such that Sum_{i=0..k} 1/A001316(i) >= n.
 * https://oeis.org/A235333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that Σ_{i=0..k} 1/A001316(i) ≥ n. Here
 *            A001316(i) = 2^{popcount(i)}. For each i the summand is
 *            2^{-popcount(i)}
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a common denominator that is a power of two.
   sum_num / 2^denominator  is the accumulated sum.
   For each i we adjust the denominator up to the largest popcount seen so far,
   then add the appropriate numerator. The comparison is done by
   "sum_num >= n << denominator". */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* not defined for n=0 but return 0 */
        return (seq_t)0;

    idx_t   i = 0;            /* current index */
    seq_t   sum_num = 0;      /* numerator of the accumulated sum   */
    unsigned den = 0;         /* current denominator exponent (2^den) */

    for (;; ++i) {
        unsigned pop = __builtin_popcountll(i);      /* popcount of i */

        /* raise denominator if necessary */
        if (pop > den) {
            sum_num <<= (pop - den);
            den = pop;
        }

        /* add the term 2^{-pop} represented with denominator 2^den */
        sum_num += (seq_t)1 << (den - pop);

        /* compare with the threshold n*2^den */
        seq_t threshold = (seq_t)n;
        threshold <<= den;

        if (sum_num >= threshold)
            return (seq_t)i;
    }
}
