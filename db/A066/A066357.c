/*
 * A066357  Number of ordered (i.e., planar) trees on 2n edges with every
 *          subtree at the root having an even number of edges.
 * https://oeis.org/A066357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1, a(1) = 1 * for n > 1, * a(n) = ( a(n-1)*(32*n^2 -
 *            64*n + 39)*2*n * - a(n-2)*(2*n-3)*(4*n-5)*(4*n-7)*16 ) * /
 *            ((2*n-1)*(n+1)*n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* Use 128-bit intermediate when seq_t is at most 64‑bit
           to avoid overflow of intermediate products. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 f1 = (unsigned __int128)(32 * i * i - 64 * i + 39) *
                                   (unsigned __int128)(2 * i);
            unsigned __int128 f2 = (unsigned __int128)(2 * i - 3) *
                                   (unsigned __int128)(4 * i - 5) *
                                   (unsigned __int128)(4 * i - 7) *
                                   (unsigned __int128)16;
            unsigned __int128 denom = (unsigned __int128)(2 * i - 1) *
                                      (unsigned __int128)(i + 1) *
                                      (unsigned __int128)i;
            unsigned __int128 term1 = (unsigned __int128)prev1 * f1;
            unsigned __int128 term2 = (unsigned __int128)prev2 * f2;
            unsigned __int128 numer = term1 - term2;
            curr = (seq_t)(numer / denom);
        } else {
            /* For arbitrary‑precision seq_t, rely on its native arithmetic. */
            seq_t f1 = ((seq_t)(32 * i * i - 64 * i + 39)) * ((seq_t)(2 * i));
            seq_t f2 = ((seq_t)(2 * i - 3)) * ((seq_t)(4 * i - 5)) *
                       ((seq_t)(4 * i - 7)) * ((seq_t)16);
            seq_t denom = ((seq_t)(2 * i - 1)) * ((seq_t)(i + 1)) *
                          ((seq_t)i);
            seq_t term1 = prev1 * f1;
            seq_t term2 = prev2 * f2;
            seq_t numer = term1 - term2;
            curr = numer / denom;
        }

        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
