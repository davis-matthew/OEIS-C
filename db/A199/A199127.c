/*
 * A199127  Number of n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order, the number of instances of each value within one of
 *          each other, and no element equal to any horizontal or vertical
 *          neighbor.
 * https://oeis.org/A199127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/3*C(3k,k)*C(2k,k) if n = 3k a(n) = C(3k+1,k+1)*C(2k,k)
 *            if n = 3k+1 a(n) = C(3k+2,k+1)*C(2k+1,k) if n = 3k+2 where
 *            C(x,y) is the binomial coefficient "x choose y". The binomial
 *            coefficients are computed multiplicatively with exact division
 *            after each step to keep intermediate values small
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 45, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t binom_small(idx_t n, idx_t k)
{
    /* Compute C(n,k) exactly using multiplicative formula. */
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first then divide; the result is always an integer. */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) and higher are defined; for n==0 the function is undefined. */
    idx_t mod = n % 3;
    if (mod == 0) {                /* n = 3k */
        idx_t k = n / 3;
        seq_t c1 = binom_small(3 * k, k);          /* C(3k,k) */
        seq_t c2 = binom_small(2 * k, k);          /* C(2k,k) */
        return c1 / (seq_t)3 * c2;                /* 1/3*C(3k,k)*C(2k,k) */
    } else if (mod == 1) {          /* n = 3k+1 */
        idx_t k = (n - 1) / 3;
        seq_t c1 = binom_small(3 * k + 1, k + 1);   /* C(3k+1,k+1) */
        seq_t c2 = binom_small(2 * k, k);           /* C(2k,k) */
        return c1 * c2;                          /* C(3k+1,k+1)*C(2k,k) */
    } else {                        /* mod == 2, n = 3k+2 */
        idx_t k = (n - 2) / 3;
        seq_t c1 = binom_small(3 * k + 2, k + 1);   /* C(3k+2,k+1) */
        seq_t c2 = binom_small(2 * k + 1, k);       /* C(2k+1,k) */
        return c1 * c2;                          /* C(3k+2,k+1)*C(2k+1,k) */
    }
}
