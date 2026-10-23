/*
 * A106464  Antidiagonal sums of number triangle A003989.
 * https://oeis.org/A106464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} gcd(n-2*k+1, k+1) (OEIS A106464)
 * Range:     u8 n <= 116, u16 n <= 6296, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_seq.h"

/* Local Euclidean GCD for seq_t */

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk = n / 2;                 /* floor(n/2)   */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term_a = (seq_t)n - ((seq_t)k << 1) + 1; /* n - 2*k + 1 */
        seq_t term_b = (seq_t)k + 1;                  /* k + 1       */
        total += gcd_seq(term_a, term_b);
    }
    return total;
}
