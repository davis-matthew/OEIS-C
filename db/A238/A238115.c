/*
 * A238115  Number of states arising in matrix method for enumerating
 *          Hamiltonian cycles on a 2n X 2n grid.
 * https://oeis.org/A238115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)^2 * catalan(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include "../utils/catalan.h"   /* provides catalan(idx_t) */

/* External function declaration for idx_t and seq_t types. */
extern seq_t catalan(idx_t k);

/* Compute a(n) for OEIS sequence A238115. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t sum = 0;
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* binom = binom(n, k) computed iteratively */
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;
        seq_t binom_sqr = binom * binom;
        seq_t cat = catalan(k);
        sum += binom_sqr * cat;
    }
    return sum;
}
