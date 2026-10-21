/*
 * A088488  a(n) = Sum_{k=1..8} floor(A254876(n,k)/A254876(n-1,k)), where
 *          A254876(n,k) = n! / (Product_{m=(n-floor((2n)/(3^k))) ..
 *          (n-floor((n)/(3^k)))} m).
 * https://oeis.org/A088488
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Sum_{k=1..8} floor( A254876(n,k) / A254876(n-1,k) ),
 *            where A254876(n,k) = n! / (∏_{m = n – ⌊2n/3^k⌋}^{\,n – ⌊n/3^k⌋}
 *            m)
 * Range:     u8 n <= 35, u16 n <= 8234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 3^k for k = 1 .. 8 */
    static const idx_t pow3[9] =
        {1, 3, 9, 27, 81, 243, 729, 2187, 6561};

    if (n < 2)
        return 0;          /* value for n < 2 is undefined, return 0 */

    idx_t nm = n - 1;       /* n-1 */
    seq_t sum = 0;

    for (int kk = 1; kk <= 8; ++kk) {
        idx_t t = pow3[kk];

        /* boundaries for n */
        idx_t L_n = n - (2 * n) / t;
        idx_t R_n = n - n / t;

        /* boundaries for n-1 */
        idx_t L_nm = nm - (2 * nm) / t;
        idx_t R_nm = nm - nm / t;

        /* numerator and denominator will fit in 128 bits (product of few terms) */
        unsigned __int128 num = n;      /* include factor n */
        unsigned __int128 den = 1;

        /* left side: terms in one interval but not the other */
        if (L_nm < L_n) {
            for (idx_t m = L_nm; m < L_n; ++m)
                num *= (unsigned __int128)m;
        }
        if (L_n < L_nm) {
            for (idx_t m = L_n; m < L_nm; ++m)
                den *= (unsigned __int128)m;
        }

        /* right side: terms in one interval but not the other */
        if (R_nm > R_n) {
            for (idx_t m = R_n + 1; m <= R_nm; ++m)
                num *= (unsigned __int128)m;
        }
        if (R_n > R_nm) {
            for (idx_t m = R_nm + 1; m <= R_n; ++m)
                den *= (unsigned __int128)m;
        }

        /* floor division */
        seq_t ratio = (seq_t)(num / den);
        sum += ratio;
    }

    return sum;
}
