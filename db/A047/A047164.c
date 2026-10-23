/*
 * A047164  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= n/2.
 * https://oeis.org/A047164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2), 3*k) *
 *            binomial(ceil(n/2), k)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 43, u64 n <= 81, u128 n <= 157,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

/* Count nonempty subsets of {1..n} in which exactly 3/4 of the elements are ≤ n/2. */
seq_t a(idx_t n)
{
    idx_t lower = n / 2;                     /* floor(n/2) */
    idx_t upper = (n + 1) / 2;               /* ceil(n/2) */
    idx_t k_max = lower / 3;                 /* maximum k satisfying 3*k ≤ lower */
    if (k_max > upper) k_max = upper;        /* also need k ≤ upper */

    seq_t total = 0;
    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t lk = 3 * k;                    /* number taken from lower half */
        seq_t b1 = binom(lower, lk);        /* C(lower, 3k) */
        seq_t b2 = binom(upper, k);         /* C(upper, k) */
        total += seq_mul(b1, b2);           /* add the product to the sum */
    }
    return total;
}
