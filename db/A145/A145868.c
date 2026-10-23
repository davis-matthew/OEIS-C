/*
 * A145868  Number of involutions of length 2n+1 which are invariant under the
 *          reverse-complement map and have no decreasing subsequences of
 *          length 7.
 * https://oeis.org/A145868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} C(n,j) * C(n-j,⌊(n-j)/2⌋) * Catalan(⌊(j+1)/2⌋)
 *            * Catalan(⌈(j+1)/2⌉)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

static inline seq_t mul_seq(seq_t a, seq_t b)
{
    /* Promote to 128‑bit to avoid overflow in small builds        */
    return (seq_t)((__uint128_t)a * b);
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t term = binomial(n, j);

        /* C(n-j, floor((n-j)/2)) */
        idx_t rem   = n - j;
        idx_t half  = rem / 2;
        term = mul_seq(term, binomial(rem, half));

        /* Catalan factors */
        idx_t cat1 = (j + 1) / 2;     /* floor((j+1)/2) */
        idx_t cat2 = (j + 2) / 2;     /* ceil((j+1)/2)  */
        term = mul_seq(term, catalan(cat1));
        term = mul_seq(term, catalan(cat2));

        result += term;
    }

    return result;
}
