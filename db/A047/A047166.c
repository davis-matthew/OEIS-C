/*
 * A047166  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= n/2.
 * https://oeis.org/A047166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} binomial(floor(n/2), 2k) * binomial(ceil(n/2),
 *            3k)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a multiplicative routine 'binom' to avoid
   external dependencies. All arithmetic stays within seq_t. */
#include <stddef.h>

/* Multiplicative binomial coefficient: binom(n, k) ≤ seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n - k)            /* use symmetry */
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* res = res * (n - k + i) / i; division is exact */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    idx_t half = n / (idx_t)2;                       /* floor(n/2)   */
    idx_t ceil_half = n - half;                      /* ceil(n/2)    */
    idx_t kmax1 = half / (idx_t)2;                   /* 2k ≤ floor(n/2) */
    idx_t kmax2 = ceil_half / (idx_t)3;              /* 3k ≤ ceil(n/2)  */
    idx_t kmax = (kmax1 < kmax2) ? kmax1 : kmax2;    /* maximum valid k */

    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= kmax; ++k) {
        idx_t choose1 = (idx_t)(2 * k);
        idx_t choose2 = (idx_t)(3 * k);
        seq_t c1 = binom(half, choose1);
        seq_t c2 = binom(ceil_half, choose2);
        sum += c1 * c2;
    }
    return sum;
}
