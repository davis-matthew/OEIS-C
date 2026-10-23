/*
 * A179534  Number of labeled split graphs on n vertices.
 * https://oeis.org/A179534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A047863(n) - n*A047863(n-1) for n >= 3, with special
 *            cases a(1) = 1, a(2) = 2. A047863(n) = Σ_{k=0..n} binomial(n,k)
 *            * 2^{k(n-k)}. The implementation uses the provided helpers
 *            pow2() and binomial()
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t
calc_A(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        idx_t exp = k * (n - k);
        seq_t pow = pow2(exp);
        sum += bin * pow;
    }
    return sum;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the official offset */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t An  = calc_A(n);
    seq_t Anm = calc_A(n - 1);
    return An - (seq_t)n * Anm;
}
