/*
 * A084641  Binomial transform of n^7.
 * https://oeis.org/A084641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*k^7 (equivalent closed form:
 *            a(n) = n^2*(n^5+21*n^4+105*n^3+35*n^2-210*n+112)*2^(n-7) for
 *            n>=7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 34, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // For n < 7 we compute by summation to avoid negative powers of two.
    if (n < 7) {
        seq_t sum = 0;
        seq_t binom = 1;              // binomial(n, 0)
        for (idx_t k = 0; k <= n; ++k) {
            // compute k^7
            seq_t kpow = 1;
            seq_t sk = (seq_t)k;
            for (int i = 0; i < 7; ++i) kpow *= sk;

            sum += binom * kpow;

            // update binom to binomial(n, k+1)
            if (k < n) {
                seq_t numerator   = binom * (seq_t)(n - k);
                seq_t denominator = (seq_t)(k + 1);
                binom = numerator / denominator;   // exact division
            }
        }
        return sum;
    }

    // For n >= 7 use the closed‑form expression.
    seq_t sn   = (seq_t)n;
    seq_t sn2  = sn * sn;                 // n^2
    seq_t sn3  = sn2 * sn;                // n^3
    seq_t sn4  = sn2 * sn2;               // n^4
    seq_t sn5  = sn4 * sn;                // n^5

    seq_t poly = sn5                       // n^5
               + (seq_t)21 * sn4            // 21 n^4
               + (seq_t)105 * sn3           // 105 n^3
               + (seq_t)35  * sn2           // 35 n^2
               - (seq_t)210 * sn            // -210 n
               + (seq_t)112;                // +112

    seq_t two_pow = (seq_t)1 << (uint64_t)(n - 7);   // 2^(n-7)

    return sn2 * poly * two_pow;
}
