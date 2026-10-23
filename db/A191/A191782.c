/*
 * A191782  Sum of the lengths of the first ascents in all n-length left
 *          factors of Dyck paths.
 * https://oeis.org/A191782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2, ⌊n/2⌋+1) - binomial(n, ⌊n/2⌋) - 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)            /* out of range */
        return 0;
    if (k > n - k)        /* use symmetry */
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i); /* current numerator   */
        seq_t den = (seq_t)i;           /* current denominator */

        /* cancel common factors with the running result first */
        seq_t g = gcd(res, den);
        res = res / g;
        den = den / g;

        /* cancel remaining factors of the denominator with the numerator */
        g = gcd(num, den);
        num = num / g;
        den = den / g;          /* now den should be 1 for valid binomial steps */

        /* multiply the remaining numerator factors */
        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n = 0 is irrelevant */
        return 0;

    idx_t k = n / 2;  /* floor(n/2) */
    seq_t t1 = binom(n + 2, k + 1);
    seq_t t2 = binom(n, k);
    return t1 - t2 - 1;
}
