/*
 * A216786  a(n) = Product_{k=1..n} (121 - 11/k).
 * https://oeis.org/A216786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (121 - 11/k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Convert k to seq_t for computations */
        seq_t k_seq = (seq_t)k;

        /* Numerator of the k‑th factor */
        seq_t num = (seq_t)121 * k_seq - (seq_t)11;

        /* Reduce res and k_seq by their gcd to avoid overflow */
        seq_t g1 = gcd(res, k_seq);
        if (g1 > (seq_t)1) {
            res /= g1;
            k_seq /= g1;
        }

        /* Reduce num and remaining k_seq by their gcd */
        seq_t g2 = gcd(num, k_seq);
        if (g2 > (seq_t)1) {
            num /= g2;
            k_seq /= g2;
        }

        /* After reductions k_seq should be 1.
           If not, perform the remaining division (should not happen). */
        res = res * num;
        if (k_seq != (seq_t)1) {
            res /= k_seq;
        }
    }
    return res;
}
