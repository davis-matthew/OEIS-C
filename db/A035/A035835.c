/*
 * A035835  Coordination sequence for lattice D*_100 (with edges defined by
 *          l_1 norm = 1).
 * https://oeis.org/A035835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A035835: Coordination sequence for lattice D*_100
   Formula: a(n) = Σ_{k=1..100} 2^k * binomial(100,k) * binomial(n-1,k-1)
            + 2^100 * binomial((100+2n)/2-1, 100-1)
   (Since (100+2n)/2-1 = n+49, the last binomial is binomial(n+49,99)) */

#include "../utils/binom.h"  /* provides static seq_t binom(idx_t a, idx_t b) */
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

/* External entry point for the sequence */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return 1;

    const idx_t N = 100;          /* upper index in the binomial sums */
    seq_t result = 0;

    /* First sum: Σ_{k=1..100} 2^k * C(100,k) * C(n-1,k-1) */
    for (idx_t k = 1; k <= N; ++k) {
        /* If k-1 > n-1, binom(n-1,k-1) returns 0, so we can stop early */
        if (k - 1 > n - 1)
            break;
        seq_t term = pow2(k);                 /* 2^k */
        term *= binom(N, k);                  /* * C(100,k) */
        term *= binom(n - 1, k - 1);          /* * C(n-1,k-1) */
        result += term;                       /* accumulate */
    }

    /* Second term: 2^100 * C(n+49,99) */
    seq_t term2 = pow2(N);                    /* 2^100 */
    term2 *= binom(n + 49, 99);              /* * C(n+49,99) */
    result += term2;

    return result;
}
