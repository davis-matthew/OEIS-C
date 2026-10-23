/*
 * A180297  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-7.
 * https://oeis.org/A180297
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weak compositions of n into n parts * with
 *            maximum part exactly n-7. * Computed via inclusion–exclusion: *
 *            f(k) = Σ_{j≥0} (-1)^j C(n,j) · C(2n – j(k+1) – 1, n–1) * a(n) =
 *            f(n-7) – f(n-8) (k is the upper bound)
 * Range:     u8 none, u16 n <= 9, u32 n <= 44, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* multiplicative binomial coefficient C(a,b), 0 ≤ b ≤ a.
 * Works for arbitrary precision seq_t (no overflow), and uses only
 * integer arithmetic (truncating division). */
static seq_t _binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b == 0 || a == b) return 1;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        /* multiply by (a - b + i), then divide by i */
        res = res * (seq_t)(a - b + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* number of weak compositions of n into n parts with each part ≤ k.
 * If k < 0 or k > n, returns 0. */
static seq_t _count_leq(idx_t n, idx_t k)
{
    if (k < 0 || k > n) return 0;
    idx_t maxj = n / (k + 1);
    seq_t total = 0;
    for (idx_t j = 0; j <= maxj; ++j) {
        /* term = C(n, j) * C(2n – j(k+1) – 1, n-1) */
        idx_t a = 2 * n - j * (k + 1) - 1;
        idx_t b = n - 1;
        if (a < b) continue;  /* binomial is zero */
        seq_t term = _binom(n, j) * _binom(a, b);
        if (j & 1)
            total -= term;
        else
            total += term;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;          /* undefined below the offset */
    idx_t k = n - 7;              /* maximum allowed */
    seq_t with_k   = _count_leq(n, k);
    seq_t with_km1 = _count_leq(n, k - 1);
    return with_k - with_km1;
}
