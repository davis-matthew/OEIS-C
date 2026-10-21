/*
 * A093680  Sequence contains no 3-term arithmetic progression, starting with
 *          1, 19.
 * https://oeis.org/A093680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} (3^{v2(k)}+1)/2 + f(n) where v2(k) is the
 *            exponent of 2 in k and f is the 16‑periodic function with values
 *            {1,18,17,18,14,18,17,18,5,18,17,18,14,19,19,19}
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t f[16] = {
    (seq_t)1, (seq_t)18, (seq_t)17, (seq_t)18,
    (seq_t)14, (seq_t)18, (seq_t)17, (seq_t)18,
    (seq_t)5,  (seq_t)18, (seq_t)17, (seq_t)18,
    (seq_t)14, (seq_t)19, (seq_t)19, (seq_t)19
};

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 1; a(0) is undefined, return 0 */
        return n == 1 ? (seq_t)1 : (seq_t)0;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k < n; ++k) {
        /* compute v2(k) : number of trailing zeros */
        idx_t ev = (idx_t)0;
        idx_t tmp = k;
        while ((tmp & (idx_t)1) == (idx_t)0) {
            ++ev;
            tmp >>= (idx_t)1;
        }

        /* compute 3^ev */
        seq_t pow3 = (seq_t)1;
        for (idx_t i = (idx_t)0; i < ev; ++i)
            pow3 *= (seq_t)3;

        /* term = (pow3 + 1)/2 */
        sum += (pow3 + (seq_t)1) / (seq_t)2;
    }

    /* add the periodic f(n) */
    size_t idx = (size_t)((n - (idx_t)1) % (idx_t)16);
    return sum + f[idx];
}
