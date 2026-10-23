/*
 * A137646  a(n) = Sum_{k=0..n} C(k*(k+1)/2, k) * C(k*(k+1)/2, n-k).
 * https://oeis.org/A137646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(k*(k+1)/2, k) * C(k*(k+1)/2, n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Small helper that computes binomial(n, k) for idx_t arguments.
   It uses the multiplicative formula and works for any seq_t that
   supports +, -, *, /, and works at compile time for arbitrary‑precision types. */
static seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n - k) /* use symmetry to reduce loop count */
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* r = r * (n - k + i) / i; the division is exact */
        r = r * (n - k + i) / i;
    }
    return r;
}

/* Entry point for the sequence. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;          /* T(k) = k(k+1)/2 */
        seq_t c1 = binom_simple((idx_t)tri, k);             /* C(tri, k)   */
        seq_t c2 = binom_simple((idx_t)tri, n - k);         /* C(tri, n-k) */
        sum += c1 * c2;
    }
    return sum;
}
