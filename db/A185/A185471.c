/*
 * A185471  Number of (n+2)X5 0..2 arrays with each 3X3 subblock having rows
 *          and columns in lexicographically nondecreasing order.
 * https://oeis.org/A185471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{13} C(n-1, k) * c_k where c_0 = 13659 c_1 =
 *            62650 c_2 = 169733 c_3 = 332378 c_4 = 485179 c_5 = 523145 c_6 =
 *            411565 c_7 = 234590 c_8 = 96850 c_9 = 29500 c_10= 7043 c_11=
 *            1401 c_12= 205 c_13= 14
 * Range:     u8 none, u16 n <= 1, u32 n <= 15, u64 n <= 132, u128 n <= 200,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1: a(1) is first term */

    static const long long coeff[14] = {
        13659LL, 62650LL, 169733LL, 332378LL,
        485179LL, 523145LL, 411565LL, 234590LL,
        96850LL, 29500LL, 7043LL, 1401LL,
        205LL, 14LL
    };

    seq_t result = 0;
    /* compute terms while k < n (since C(n-1, k) = 0 for k >= n) */
    for (idx_t k = 0; k < 14 && k < n; ++k) {
        seq_t b = binom(n - 1, k);              /* binomial coefficient */
        seq_t term = mul_small(b, (unsigned)coeff[k]); /* multiply by small integer */
        result += term;
    }
    return result;
}
