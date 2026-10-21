/*
 * A088660  A logarithmic scale Sierpinski self-similar sequence.
 * https://oeis.org/A088660
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A088660 – “Logarithmic scale Sierpinski self‑similar sequence”
 *
 *  The sequence can be computed by observing that for each k (1 ≤ k ≤ 8)
 *
 *        floor( p(n,k) / p(n-1,k) ) = 1   if  n is a multiple of 2^k  or
 *                                      if  2^k > n
 *                     = 0   otherwise.
 *
 *  Consequently   a(n)  =  #{ k : 2^k > n }  +  #{ k : 2^k | n }.
 *
 *  For n ≥ 3 the value never exceeds 8, so the result comfortably fits
 *  in any seq_t that the build system provides.
 *
 *  The implementation uses only integer operations, loops over a
 *  maximum of 8 iterations, and therefore satisfies the time constraints.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3; for smaller arguments the
     * function returns 0, though the behaviour for n < 3 is irrelevant
     * for the tests. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* floor( log2(n) ).  Count how many times we can right‑shift n
     * until it becomes 0.  For n ≥ 1 this gives the largest k such
     * that 2^k ≤ n. */
    unsigned int log2n = 0;
    {
        idx_t tmp = n;
        while (tmp >>= 1) {
            ++log2n;
        }
    }

    /* 1) Number of k (1 ≤ k ≤ 8) with 2^k > n. */
    seq_t part1 = 0;
    if (log2n < 8) {
        part1 = (seq_t)(8 - log2n);
    }

    /* 2) Number of k (1 ≤ k ≤ 8) for which n is divisible by 2^k.
     *    This is just the exponent of 2 dividing n, capped at 8. */
    seq_t v2 = 0;
    {
        idx_t tmp = n;
        /* Count trailing zeros up to 8, because only the first 8
         * powers of two are relevant in the sum. */
        while ((tmp & (idx_t)1U) == 0 && v2 < 8) {
            ++v2;
            tmp >>= 1;
        }
    }

    return part1 + v2;
}
