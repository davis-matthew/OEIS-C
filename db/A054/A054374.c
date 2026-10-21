/*
 * A054374  Discriminant of Hermite polynomials.
 * https://oeis.org/A054374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(3*n*(n-1)/2) * Π_{k=1..n} k^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

/* Computes a(n) for n >= 1.  Uses big_pow for k^k and pow2 for the power
   of two.  Values are stored in seq_t, which may be any integer type
   supported by the build (e.g. uint64_t, unsigned __int128, or an
   arbitrary‑precision class). */
seq_t a(idx_t n)
{
    /* Compute the factor 2^(3*n*(n-1)/2). */
    seq_t exp = (seq_t)n * (seq_t)(n - 1) / 2; /* (n*(n-1))/2, integer division */
    exp = exp * 3;                            /* times 3 */
    seq_t pow2_part = pow2((idx_t)exp);

    /* Compute Π_{k=1..n} k^k. */
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = big_pow((seq_t)k, (seq_t)k);
        prod = seq_mul(prod, term);
    }

    /* Multiply by the power of two part. */
    return seq_mul(prod, pow2_part);
}
