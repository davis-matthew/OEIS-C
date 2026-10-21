/*
 * A057650  Second step in Goodstein sequences, i.e., g(4) if g(2)=n: (first
 *          step) write g(2)=n in hereditary representation base 2, bump to
 *          base 3, then subtract 1 to produce g(3)=A056004(n), then (second
 *          step) write g(3) in hereditary representation base 3, bump to base
 *          4, then subtract 1 to produce g(4).
 * https://oeis.org/A057650
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the second step of the Goodstein sequence starting *
 *            with g(2)=n: g(3)=hereditary_eval(g(2),2,3)-1, *
 *            g(4)=hereditary_eval(g(3),3,4)-1. This function returns g(4) for
 *            * every n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Evaluate t in hereditary base `old_base`, then interpret the
 * expression in base `new_base`.  The result fits in seq_t. */
static seq_t eval_hereditary(seq_t t, idx_t old_base, idx_t new_base)
{
    /* Base case: if t < old_base, the hereditary representation is just
     * t copies of 1, which evaluates to t in any base. */
    if (t < old_base) {
        return t;
    }

    /* Find the largest k with old_base^k <= t.  Compute the power
     * iteratively to avoid exponentiation tables. */
    seq_t power = (seq_t)old_base;   /* old_base^1 */
    idx_t k = 1;
    while (power * (seq_t)old_base <= t) {
        power = power * (seq_t)old_base;
        k++;
    }

    /* Split t = q*power + r with 0 <= r < power and q < old_base. */
    seq_t q = t / power;
    seq_t r = t % power;

    /* Coefficient q is already < old_base < new_base, so its
     * representation at new_base is q itself.  For uniformity we
     * still call eval_hereditary, which will terminate immediately. */
    seq_t coeff = eval_hereditary((seq_t)q, old_base, new_base);

    /* Exponent k may need to be expressed recursively. */
    seq_t exp_val = eval_hereditary((seq_t)k, old_base, new_base);

    /* Compute new_base^exp_val, then multiply by the coefficient. */
    seq_t term = big_pow((seq_t)new_base, exp_val);
    term = term * coeff;

    /* Recursively evaluate the remainder r. */
    seq_t rest = eval_hereditary(r, old_base, new_base);

    return term + rest;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 2 in correct use.  We ignore indices
     * below the offset. */
    if (n < 2) {
        return 0;
    }

    /* First step: convert g(2) = n to g(3). */
    seq_t g3 = eval_hereditary((seq_t)n, 2, 3) - 1;

    /* Second step: convert g(3) to g(4). */
    seq_t g4 = eval_hereditary(g3, 3, 4) - 1;

    return g4;
}
