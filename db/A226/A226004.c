/*
 * A226004  Number of pairs of orthogonal (-6,7) vectors of length 13*n.
 * https://oeis.org/A226004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{s=0}^n ( C(13n, 7(n−s)) * C(13n−7(n−s), 6s) *
 *            2^{6n+s} )
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of unordered pairs of orthogonal (-6,7) vectors
   of length 13*n. The formula follows from counting the numbers of
   coordinate-wise equal and unequal entries that satisfy the dot‑product
   condition.  For each n a double sum over the possible numbers of
   equal 7 or -6 entries yields the closed form above. The result is
   divided by two because the superficial binomial count gives ordered
   pairs. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t N = 13 * n;          /* length of the vectors      */
    seq_t sum = 0;
    for (idx_t s = 0; s <= n; ++s)
    {
        idx_t A = 7 * (n - s); /* v_i = w_i = -6 count      */
        idx_t B = 6 * s;       /* v_i = w_i = 7   count      */
        idx_t T = 6 * n + s;   /* mixed positions count      */

        /* multinomial coefficient * 2^T   */
        seq_t term = binomial(N, A);
        term = term * binomial(N - A, B);
        term = term * pow2(T);
        sum = sum + term;
    }
    return sum / 2;            /* unordered pairs            */
}
