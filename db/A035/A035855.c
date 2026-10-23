/*
 * A035855  Coordination sequence for A_29 lattice.
 * https://oeis.org/A035855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = Σ_{d=1}^{29} C(30, d) · C(n-1, d-1) · C(29-d+n,
 *            n) for n ≥ 1 (here m = 2·n is the even norm, so n = m/2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

/* Coordination sequence for A_29 lattice */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t n_minus1 = n - 1;
    /* For d > n the second binomial becomes zero, so we can stop early. */
    for (idx_t d = 1; d <= 29; ++d) {
        if (d - 1 > n_minus1) break;

        seq_t term1 = binomial(30, d);
        seq_t term2 = binomial(n_minus1, d - 1);
        seq_t term3 = binomial(29 - d + n, n);

        seq_t prod = seq_mul(term1, term2);
        prod = seq_mul(prod, term3);

        sum = sum + prod;
    }
    return sum;
}
