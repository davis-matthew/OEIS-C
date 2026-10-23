/*
 * A188256  Number of arrangements of 10 nonzero numbers x(i) in -n..n with
 *          the sum of x(i)*x(i+1) equal to zero.
 * https://oeis.org/A188256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{9} C(n-1,k) Δ^k a(1) where the finite
 *            differences Δ^k a(1) of the first ten terms are: Δ^0 = 0 Δ^1 =
 *            57808 Δ^2 = 1542016 Δ^3 =15466168 Δ^4 =52381664 Δ^5 =159325624
 *            Δ^6 =-47931804 Δ^7 =832498524 Δ^8 =-2684244528 Δ^9 =9122230100
 *            This is the unique polynomial of degree ≤9 that matche...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

/* Helper: compute binomial coefficient C(n,k) for 0 <= k <= 9 */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k && n >= i; ++i)
    {
        res = res * (n - i + 1);
        res = res / i;          /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Finite differences Δ^k a(1) */
    static const int64_t diff[10] = {
        0,
        57808,
        1542016,
        15466168,
        52381664,
        159325624,
        -47931804,
        832498524,
        -2684244528LL,
        9122230100LL
    };

    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of absolute values of negative contributions */

    idx_t m = n - 1; /* binomials use (n-1) */

    /* k = 0 term (Δ^0 = 0) is skipped */
    for (idx_t k = 1; k <= 9; ++k)
    {
        if (m < k)               /* C(m,k) = 0 for m < k */
            break;

        seq_t bin = binom_small(m, k);          /* C(m,k) */
        seq_t term = bin * (seq_t)(diff[k] >= 0 ? diff[k] : -diff[k]);

        if (diff[k] >= 0)
            pos += term;
        else
            neg += term;
    }

    /* The result is non‑negative, so pos >= neg */
    assert(pos >= neg);
    return pos - neg;
}
