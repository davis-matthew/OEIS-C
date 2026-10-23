/*
 * A162747  A factorial-Pascal matrix.
 * https://oeis.org/A162747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A162747 – factorial‑Pascal matrix (flattened triangle).
    T(n,k) = Σ_{j=0..k} C(n,j) * C(n-j,2(k-j)) * (k-j)!,
    a(m) = T(r,k) where r satisfies r(r+1)/2 ≤ m < (r+1)(r+2)/2
           and k = m – r(r+1)/2.
    The implementation uses only integer arithmetic and pre‑computed
    factorials to stay well within the limits of the chosen seq_t type. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* factorial of n (n ≤ r, r is small enough for seq_t or for divisor use). */
static seq_t factorial_u64(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;
    return res;
}

/* binomial coefficient C(n,k) using the multiplicative formula. */
static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    return res;
}

/* Compute the m‑th term of A162747 (flattened triangle). */
seq_t a(idx_t m)
{
    /* determine the row r such that r(r+1)/2 ≤ m < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= m)
        ++r;

    /* column index k within that row */
    idx_t k = m - r * (r + 1) / 2;    /* 0 ≤ k ≤ r */

    /* pre‑compute factorials up to r to evaluate the formula efficiently */
    seq_t *fact = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!fact) return 0;              /* out of memory, return 0 as safe fallback */
    fact[0] = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        idx_t two = 2 * (k - j);          /* 2(k-j) */
        if (two > r - j)                  /* binomial term would be zero */
            continue;

        idx_t idx = r - 2 * k + j;        /* r - j - 2(k-j) = r - 2k + j */
        seq_t term = fact[r];
        term /= fact[j];                  /* divide by j!  */
        term /= fact[two];                /* divide by (2(k-j))!  */
        term /= fact[idx];                /* divide by (r-2k+j)!  */
        term *= fact[k - j];              /* multiply by (k-j)!    */
        sum += term;
    }

    free(fact);
    return sum;
}
