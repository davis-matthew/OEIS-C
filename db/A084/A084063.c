/*
 * A084063  First subdiagonal of number array A084061.
 * https://oeis.org/A084063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n - sqrt(n+1))^n + (n + sqrt(n+1))^n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* compute n^n in seq_t */
    seq_t n_pow = 1;
    seq_t sn = (seq_t)n;
    for (idx_t i = 0; i < n; ++i) {
        n_pow *= sn;
    }

    seq_t pow_nplus1 = 1;   /* (n+1)^j */
    seq_t binom = 1;        /* C(n,2j) */
    seq_t sum = 0;
    idx_t maxj = n / 2;
    seq_t n_sq = sn * sn;   /* n^2 for dividing n_pow */

    for (idx_t j = 0; j <= maxj; ++j) {
        /* add term for current j */
        sum += binom * n_pow * pow_nplus1;

        if (j == maxj) break;          /* last term processed */

        /* prepare for next j */
        n_pow = n_pow / n_sq;          /* n_pow = n_pow / n^2  */
        pow_nplus1 *= (seq_t)(n + 1);  /* increase power of (n+1) */

        /* update binomial coefficient C(n,2(j+1)) from C(n,2j) */
        seq_t num   = binom * (seq_t)(n - 2 * j) * (seq_t)(n - 2 * j - 1);
        seq_t denom = (seq_t)(2 * j + 1) * (seq_t)(2 * j + 2);
        binom = num / denom;
    }

    return sum;
}
