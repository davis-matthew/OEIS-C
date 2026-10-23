/*
 * A140414  Triangle T(p,s) showing the coefficients of sequences which are
 *          half their p-th differences.
 * https://oeis.org/A140414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   Let p be the row index such that 1 + p(p+1)/2 <= n < 1 +
 *            (p+1)(p+2)/2. Let s = n - (1 + p(p+1)/2) (0 <= s <= p). Then if
 *            s == p -> a(n) = (p even ? 3 : 1) else a(n) = (-1)^s * C(p+1,
 *            s+1) where C is the ordinary binomial coefficient
 * Range:     i8 n <= 48, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t binom_ui(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - i + 1) / i;
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset starts at 1 */

    /* Find p such that 1 + p(p+1)/2 <= n < 1 + (p+1)(p+2)/2 */
    idx_t m = n - 1;                /* n-1 so we can use p(p+1)/2 <= m */
    idx_t p = 0;
    while ((p * (p + 1)) / 2 <= m) ++p;
    --p;   /* now (p*(p+1))/2 <= m < ((p+1)*(p+2))/2 */

    idx_t start = 1 + (p * (p + 1)) / 2;   /* first index of row p */
    idx_t s = n - start;                    /* offset within the row, 0..p */

    if (s == p) /* last coefficient of the row */
        return (p % 2 == 0 ? (seq_t)3 : (seq_t)1);

    /* Compute (-1)^s * C(p+1, s+1) */
    seq_t res = binom_ui(p + 1, s + 1);
    if (s % 2 == 1)
        res = -res;
    return res;
}
