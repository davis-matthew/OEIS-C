/*
 * A159068  a(n) = Sum_{k=1..n} binomial(n,k) * gcd(k,n).
 * https://oeis.org/A159068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * gcd(k,n)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function calculates the OEIS sequence A159068 for any n >= 1. */
/* It uses the provided gcd() routine and performs an iterative update of */
/* the binomial coefficient C(n,k). All arithmetic is performed in seq_t, */
/* ensuring exactness for arbitrary precision builds. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 0;                     /* accumulator for the sum       */
    seq_t seq_n  = (seq_t)n;              /* n promoted to seq_t            */
    seq_t bin    = 1;                     /* C(n,0) = 1                     */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update bin from C(n,k-1) to C(n,k) */
        bin = bin * (seq_n - (seq_t)(k - 1));
        bin = bin / (seq_t)k;             /* exact division for binomial   */

        /* Compute gcd(k,n) */
        seq_t g = gcd((seq_t)k, seq_n);

        /* Add bin * g to the running total */
        result += bin * g;
    }

    return result;
}
