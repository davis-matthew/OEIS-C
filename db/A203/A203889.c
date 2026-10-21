/*
 * A203889  Number of (n+1)X2 0..4 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) - 348*a(n-2) + 2536*a(n-3) - 12058*a(n-4) +
 *            39384*a(n-5) - 90584*a(n-6) + 147848*a(n-7) - 170061*a(n-8) +
 *            134588*a(n-9) - 69668*a(n-10) + 21216*a(n-11) - 2880*a(n-12)
 *            Initial terms: a(1)=205, a(2)=1940, a(3)=16842, a(4)=131432,
 *            a(5)=940270, a(6)=6287755, a(7)=39889619, a(8)=24272...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)205ULL,
        (seq_t)1940ULL,
        (seq_t)16842ULL,
        (seq_t)131432ULL,
        (seq_t)940270ULL,
        (seq_t)6287755ULL,
        (seq_t)39889619ULL,
        (seq_t)242721211ULL,
        (seq_t)1428279867ULL,
        (seq_t)8179225950ULL,
        (seq_t)45807549072ULL,
        (seq_t)251870313730ULL
    };

    if (n <= 12) return init[n - 1];

    /* coefficients of the recurrence (positive or negative) */
    static int coeff[12] = {
        28, -348, 2536, -12058,
        39384, -90584, 147848, -170061,
        134588, -69668, 21216, -2880
    };

    seq_t prev[12];
    for (idx_t i = 0; i < 12; ++i) prev[i] = init[i];

    seq_t next;
    for (idx_t cur = 13; cur <= n; ++cur) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        /* compute a(cur) from previous 12 values */
        for (int j = 0; j < 12; ++j) {
            seq_t val = prev[11 - j];   /* prev[11] = a(cur-1), prev[0] = a(cur-12) */
            int c = coeff[j];
            if (c >= 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }
        next = pos - neg;  /* pos > neg is guaranteed by the recurrence */

        /* shift buffer left, drop oldest value, insert new value */
        for (idx_t i = 0; i < 11; ++i) prev[i] = prev[i + 1];
        prev[11] = next;
    }

    return next;
}
