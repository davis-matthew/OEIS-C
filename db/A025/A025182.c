/*
 * A025182  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is an
 *          integer, s(0) = 0, |s(1)| = 1, |s(i) - s(i-1)| <= 1 for i >= 2,
 *          s(n) = 4. Also a(n) = T(n,n-4), where T is the array defined in
 *          A025177.
 * https://oeis.org/A025182
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   -(n-4)*(n+4)*(n^2-3*n+6)*a(n)
 *            +(n-1)*(2*n^3-5*n^2+11*n-36)*a(n-1)
 *            +3*(n-1)*(n-2)*(n^2-n+4)*a(n-2)=0, base values a(4)=1, a(5)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 20000  /* amount sufficient for n up to ~10000 */

static seq_t cache[MAX_N + 1];
static idx_t computed = 3;   /* last computed index in cache */

/* ensure cache contains values up to at least n */
static void ensure_up_to(idx_t n)
{
    if (n > computed)
    {
        /* initialise base values once */
        if (computed < 4)
        {
            cache[4] = (seq_t)1;
            cache[5] = (seq_t)4;
            computed = 5;
        }

        for (idx_t i = computed + 1; i <= n; ++i)
        {
            seq_t sn   = (seq_t)i;
            seq_t sn2  = sn * sn;
            seq_t sn3  = sn2 * sn;

            seq_t coeffA = -(sn - (seq_t)4) * (sn + (seq_t)4) * (sn2 - (seq_t)3 * sn + (seq_t)6);
            seq_t coeffB = (sn - (seq_t)1) *
                           ((seq_t)2 * sn3 - (seq_t)5 * sn2 + (seq_t)11 * sn - (seq_t)36);
            seq_t coeffC = (seq_t)3 * (sn - (seq_t)1) * (sn - (seq_t)2) *
                           (sn2 - sn + (seq_t)4);

            seq_t numerator = coeffB * cache[i - 1] + coeffC * cache[i - 2];
            seq_t denom     = -coeffA;   /* coeffA is negative */
            cache[i] = numerator / denom;
        }
        computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;        /* behaviour below offset not defined */
    if (n > MAX_N) return (seq_t)0;    /* out of bound, should not happen */
    ensure_up_to(n);
    return cache[n];
}
