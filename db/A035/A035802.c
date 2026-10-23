/*
 * A035802  Coordination sequence for lattice D*_34 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..34} 2^k * C(34,k) * C(n-1, k-1) + 2^34 * C(n+16,
 *            33) (C is the binomial coefficient. The term with k=0 is 0 and
 *            can be omitted.) For n=0 the value is defined as 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t pow2[35];
    static seq_t binom34[35];

    if (!init) {
        /* powers of two up to 2^34 */
        pow2[0] = (seq_t)1;
        for (idx_t i = 1; i <= 34; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;

        /* binom34[k] = C(34,k) for k=0..34 */
        binom34[0] = (seq_t)1;
        for (idx_t k = 1; k <= 34; ++k) {
            /* binom34[k] = binom34[k-1] * (34 - k + 1) / k */
            seq_t num = binom34[k - 1] * (seq_t)(34 - k + 1);
            binom34[k] = num / (seq_t)k;
        }
        init = true;
    }

    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    /* --- First summation --- */
    idx_t limit = (n < 34) ? n : 34;   /* k ranges 1..limit */
    for (idx_t k = 1; k <= limit; ++k) {
        /* C(n-1, k-1) computed multiplicatively */
        seq_t binom_n = (seq_t)1;
        for (idx_t i = 1; i < k; ++i) {
            /* binom_n = binom_n * (n-1-(i-1)) / i */
            seq_t factor = (seq_t)(n - 1) - (seq_t)(i - 1);
            binom_n = (binom_n * factor) / (seq_t)i;
        }

        /* term = 2^k * C(34,k) * binom_n */
        seq_t term = pow2[k] * binom34[k];
        term = term * binom_n;
        sum += term;
    }

    /* --- Second term: 2^34 * C(n+16, 33) --- */
    if (n >= 17) {   /* only needed when n+16 >= 33 */
        seq_t binom_m = (seq_t)1;
        seq_t base = (seq_t)n + (seq_t)16;
        for (idx_t i = 1; i <= 33; ++i) {
            /* binom_m = binom_m * (base-(i-1)) / i */
            seq_t factor = base - (seq_t)(i - 1);
            binom_m = (binom_m * factor) / (seq_t)i;
        }
        seq_t term2 = pow2[34] * binom_m;
        sum += term2;
    }

    return sum;
}
