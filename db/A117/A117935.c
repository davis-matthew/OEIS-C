/*
 * A117935  Triangle, row terms converge to the Pell sequence.
 * https://oeis.org/A117935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T[i,j] where * i = s - p + 1, j = p, * s = min{s |
 *            s*(s+1)/2 >= n}, * p = n - (s-1)*s/2. * The triangle entry
 *            T[i,j] is * T[i,j] = Σ_{k=0}^{i-1} binomial(j-1, k) * 2^{⌊k/2⌋}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool pow2_inited = false;
    static seq_t pow2[128];
    if (!pow2_inited) {
        pow2[0] = (seq_t)1;
        for (int i = 1; i < 128; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
        pow2_inited = true;
    }

    if (n == 0) return (seq_t)0;

    /* find s such that s*(s+1)/2 >= n */
    idx_t s = 0;
    while (((__int128)s * (s + 1) / 2) < n) ++s;

    idx_t prev_sum = (idx_t)((( __int128)(s - 1) * s) / 2);
    idx_t p = n - prev_sum;          /* 1 <= p <= s */
    idx_t i = s - p + 1;              /* row index (1..s) */
    idx_t j = p;                      /* column index (1..s) */

    /* compute T[i,j] */
    idx_t limit = i - 1;
    if (limit > j - 1) limit = j - 1;

    seq_t comb = (seq_t)1;
    seq_t result = 0;
    for (idx_t k = 0; k <= limit; ++k) {
        int l = (int)(k >> 1);            /* floor(k/2) */
        result += comb * pow2[l];
        if (k < limit) {
            /* binomial coefficient update: C(j-1,k+1) = C(j-1,k) * (j-1-k)/(k+1) */
            comb = comb * ( (seq_t)(j - 1 - k) ) / (k + 1);
        }
    }
    return result;
}
