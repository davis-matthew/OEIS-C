/*
 * A008269  Number of strings on n symbols in Stockhausen problem.
 * https://oeis.org/A008269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula for A008269:
   a(n) = Sum_{k=0..n} (2k-1)!! * n!/(n-k)!,
   where (2k-1)!! = product_{j=1..k} (2j-1) (with (–1)!! = 1).
   Using the recurrence
     term_0 = 1
     term_k = term_{k-1} * (2k-1) * (n-k+1),
   we can compute a(n) in O(n) time.  All operations are integer
   arithmetic on type seq_t (unsigned 8/16/32/64 or arbitrary-precision). */

#include <stdint.h>

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)9;

    seq_t term = (seq_t)1;   /* term_0 */
    seq_t sum  = term;       /* running sum of terms */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* term = term * (2k-1) * (n - k + 1) */
        term = term * ((seq_t)(2 * k - 1));
        term = term * ((seq_t)(n - k + 1));
        sum += term;
    }

    return sum;
}
