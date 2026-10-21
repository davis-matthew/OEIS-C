/*
 * A099127  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3,4,5,6,7,8}.
 * https://oeis.org/A099127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n+8, n)+n-1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute C(M, k) for arbitrary‐precision seq_t.
 * M and the result are in seq_t; k is small enough to fit in idx_t.
 * This routine performs the multiplication / division in a
 * numerically stable way using GCD reductions at each step. */
static seq_t
large_binom(seq_t M, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* numerator term: M - k + i */
        seq_t num = M - (seq_t)(k - i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(res, den);
        res /= g2;
        den /= g2;            /* should now be 1 */

        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* first binomial: C(n+8, 8) == C(n+8, n) */
    seq_t first = large_binom(n + 8, 8);

    /* second binomial: C(first + n - 1, n) */
    seq_t m = first + (seq_t)n - 1;
    return large_binom(m, n);
}
