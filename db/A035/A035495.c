/*
 * A035495  Number of musical scales consisting of n notes.
 * https://oeis.org/A035495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   a(n) = coefficient of x^n in the cycle index of C12 with *
 *            substitution 1+x. Equivalently, the number of binary necklaces
 *            of * length 12 having exactly n ones: * a(n) = (1/12) * Σ_{d |
 *            12, d | n} φ(d) * C(12/d, n/d)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply combinatorially: C(n,k) for 0 <= k <= n */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* any scale with more than 12 notes is impossible in the 12‑tone
       equal temperament */
    if (n > (idx_t)12) return (seq_t)0;

    /* divisors of 12 and their Euler phi values */
    static const idx_t DIV[6]   = {1, 2, 3, 4, 6, 12};
    static const idx_t PHI[6]   = {1, 1, 2, 2, 2, 4};

    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)0; i < (idx_t)6; ++i) {
        idx_t d = DIV[i];
        if (n % d == (idx_t)0) {
            idx_t k = n / d;        /* number of ones in each period   */
            idx_t m = (idx_t)12 / d;/* period length                    */
            sum += (seq_t)PHI[i] * binomial(m, k);
        }
    }

    return sum / (seq_t)((idx_t)12);   /* divide by the group order      */
}
