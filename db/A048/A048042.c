/*
 * A048042  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{min(⌊L/3⌋, H)} C(L, 3*t) * C(H, t) where L =
 *            floor((n+1)/3), H = n - L. Count non‑empty subsets of {1,…,n}
 *            whose 3/4 of the elements are ≤ (n+1)/3
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Multiplicative binomial coefficient using gcd to keep intermediates
   small and correct for arbitrary‑precision seq_t. */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t numer = (seq_t)(N - i + 1);
        seq_t denom = (seq_t)i;

        /* Reduce res and denom by gcd */
        seq_t g = gcd(res, denom);
        res /= g;
        denom /= g;

        /* Reduce numer and denom by gcd */
        g = gcd(numer, denom);
        numer /= g;
        denom /= g;   /* denom now 1 */

        res *= numer;   /* multiply by remaining numerator */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t L = (n + 1) / 3;   /* number of elements ≤ (n+1)/3   */
    idx_t H = n - L;         /* number of elements  > (n+1)/3   */

    idx_t max_t = (L / 3 < H ? L / 3 : H);  /* t ≤ ⌊L/3⌋ and t ≤ H    */
    seq_t total = 0;

    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t term = binom(L, 3 * t);
        term *= binom(H, t);
        total += term;
    }

    return total;
}
