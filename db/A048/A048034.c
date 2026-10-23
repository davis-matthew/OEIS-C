/*
 * A048034  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=5,10,15,...,n} [C(t,3k/5) * C(n-t,2k/5)] where t =
 *            floor((n-3)/3). The summand is included only if 3k/5 <= t and
 *            2k/5 <= n-t. a(0) is undefined; a(1) = 0
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient using seq_t arithmetic. */
static seq_t binom_custom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i */
        res = res * ((seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute threshold t = floor((n-3)/3). */
    idx_t t = (n >= 3) ? (n - 3) / 3 : 0;
    idx_t n1 = t;
    idx_t n2 = n - t;

    seq_t total = (seq_t)0;

    for (idx_t k = 5; k <= n; k += 5) {
        idx_t a_cnt = (3 * k) / 5;
        idx_t b_cnt = (2 * k) / 5;

        if (a_cnt <= n1 && b_cnt <= n2) {
            seq_t part1 = binom_custom(n1, a_cnt);
            seq_t part2 = binom_custom(n2, b_cnt);
            total += part1 * part2;
        }
    }
    return total;
}
